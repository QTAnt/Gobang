#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "PlayerManager.hpp"
#include <pthread.h>
//#include "RoomManager.hpp"

#define MATCH_LEVEL 101
using namespace std;

class Hall{
    private:
        PlayerManager pm;
        vector<vector<uint32_t> > match_pool;
        int match_num;
        pthread_mutex_t match_lock;
        pthread_cond_t match_lock;
        //RoomManager rm;

    public:

        Hall():vector(MATCH_LEVEL), match_num(0)
        {

        }

        int MatchNum()
        {
            return match_num;
        }

        void IncMatchNum()
        {
            match_num++;
        }

        void ResetMatchNum()
        {
            match_num = 0;
        }

        void GetAllMatchId(vector<uint32_t > &id_list)
        {
            for(auto i = MATCH_LEVEL-1; i>= 0; i--){
                auto &v = match_pool[i]; //拿到胜率为i的所有用户
                if(v.empty()){
                    continue;
                }
                for(auto it = v.begin(); it != v.end(); it++){
                    id_list.push_back(*it);
                }
            } 

        }

        void LockMatchPool()
        {
            pthread_mutex_lock(&match_lock);
        }

        void UnlockMatchPool()
        {
            pthread_mutex_unlock(&match_lock);
        }

        void ServiceWait()
        {
            pthread_cond_wait(&match_cond, &match_lock);
        }

        void ServiceWakuup()
        {
            pthread_cond_signal(&match_cond);
        }

        uint32_t Register(string &name, string &passwd)
        {
            return pm.InsertPlayer(name, passwd);
        }

        uint32_t Login(uint32_t &id, string &passwd)
        {
            return pm.SearchPlayer(id, passwd);
        }

        bool PushIdInMatchPool(uint32_t &id)
        {
            //LOG(INFO, "Push Player Into Match Pool...");
            
            pm.SetMatching(id); //匹配成功后状态更改
            //
            //将用户全部放入匹配池中
            int rate = pm.GetRate(id);
            LockMatchPool();
            auto &v = match_pool[rate]; //同一级别的v
            auto it = v.begin(); //判断当前用户的是否之前存储进匹配池过
            for(; it != v.end(); it++){
                if(*it == id){
                    return false;
                }
            }
            v.push_back(id); //直接放入匹配池
            IncMatchPool();
            UnlockMatchPool();
            return pm.PlayerWait(id);//可超时等待，一旦超时，匹配失败

        }

        void MatchPoolClear()
        {
             for(auto i = MATCH_LEVEL-1; i>= 0; i--){
                auto &v = match_pool[i]; //拿到胜率为i的所有用户
                if(v.empty()){
                    continue;
                }

                vector<uint32_t>().swap(v);
             }
             ResetMatchNum(); 
        }
        //存在一个线程：周而复始的检测匹配池中是否有匹配的用户
        static void *MatchService(void *arg)
        {
            pthread_detach(pthread_self());
            Hall *hp = (Hall*)arg;

            hp->LockMatchPool();
            while(hp->MatchNum() < 2){
                //wait
                ServiceWait();
            }

            //将所有用户放进id_list里
            vector<uint32_t> id_list;
            hp->GetAllMatchId(id_list);
            int num = id_list.size();
            num &= (~1);
            for(auto i = 0; i <= num ; i+=2){
                //auto &v = match_pool[i]; //拿到胜率为i的所有用户
                uint32_t play_one = id_list[i];
                uint32_t playa_two = id_list[i+1];
                
                //匹配成功
                
                pm.SetPlayStatus(play_one, play_two);
                
                //CreateRoom(play_one, play_two);
                pm.SignalPlayer(play_one, play_two);
            }
           hp->MatchPoolClear();
           hp->UnlockMatchPool();

            //两两匹配

        }

        void InitHall()
        {
            pthread_mutex_init(&match_lock, NULL);
            pthread_cond_init(&match_cond, NULL);

            pthread_t tid;
            pthread_create(&tid, NULL, MatchService, this);
        }
        ~Hall()
        {

            pthread_mutex_destroy(&match_lock);
            pthread_cond_destroy(&match_cond);
        }
};
