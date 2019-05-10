#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "PlayerManager.hpp"
#include <pthread.h>
#include "RoomManager.hpp"

#define MATCH_LEVEL 101

using namespace std;

class Hall{
    private:
        PlayerManager pm;
        vector<vector<uint32_t> > match_pool;
        int match_num;
        pthread_mutex_t match_lock;
        pthread_cond_t match_cond;
        RoomManager rm; //房间管理

    public:

        Hall()
			:match_pool(MATCH_LEVEL)
			, match_num(0)
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
                auto &v = match_pool[i]; //拿到胜率为i的所有用户(同等胜率的用户)
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

        void ServiceWakeup()
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
            LOG(INFO, "Push Player Into Match Pool...");
            pm.SetMatching(id); //匹配成功后状态更改
            //
            //将用户全部放入匹配池中
            int rate = pm.GetRate(id);
            cout << "rate: " << rate  << endl;
            LockMatchPool();
            auto &v = match_pool[rate]; //同一级别的v
            auto it = v.begin(); //判断当前用户的是否之前存储进匹配池过
            for(; it != v.end(); it++){
                if(*it == id){
                    return false;
                }
            }
            v.push_back(id); //直接放入匹配池
            IncMatchNum();//增加匹配的人数
            UnlockMatchPool();
            ServiceWakeup();  //匹配线程被唤醒，将匹配池里的id进行两两匹配
            return true;
            //return pm.PlayerWait(id);//可超时等待，一旦超时，匹配失败

        }

        //生产者消费者模型
		bool PopIdMatchPool(uint32_t &id)
        {
            LockMatchPool();
            MatchPoolClearClear(id);
            UnlockMatchPool();
        }
        //清空匹配池的内容
        void MatchPoolClear(int32_t &id)
        {
             LOG(INFO, "match pool clear...");
             for(auto i = MATCH_LEVEL-1; i>= 0; i--){
                auto &v = match_pool[i]; //拿到胜率为i的所有用户
                if(v.empty()){
                    continue;
                }

                vector<uint32_t>().swap(v);
             }
             ResetMatchNum();
             if(id >= 10000){
                 pm.SetOnline(id);
             }
        }

        int IsPlayerReady(uint32_t &id)
        {
            return pm.Ready(id);
        }

        void GamePrepare(uint32_t &one, uint32_t &two)
        {
             pm.SetPlayStatus(one, two);
             uint32_t room_id = rm.CreateRoom(one, two);
             pm.SetPlayRoom( room_id, one, two);
             //pm.SignalPlayer(one, two);
        }


        //存在一个线程：周而复始的检测匹配池中是否有匹配的用户
        static void *MatchService(void *arg)
        {
            pthread_detach(pthread_self());//先分离，获得大厅的属性
            Hall *hp = (Hall*)arg;

            while(1){
                uint32_t last = 0;
                hp->LockMatchPool();
                while(hp->MatchNum() < 2){
                    LOG(INFO, "service thread wait...");
                    //wait
                    hp->ServiceWait();
                 }

                //将所有用户放进id_list里
                LOG(INFO, "service thread wakeup..."); //匹配线程数目大于2
                //匹配
                vector<uint32_t> id_list;
                hp->GetAllMatchId(id_list);//获得大小(偶数，可匹配成功；奇数，考虑匹配一个人机)
                int num = id_list.size();
                if(num & 1){
                    last = id_list[id_list.size()-1];
                    num &= (~1);
                }else{
                    last = 0;//last最后匹配失败的那个玩家
                }
                for(auto i = 0; i <= num ; i+=2){
                    //auto &v = match_pool[i]; //拿到胜率为i的所有用户
                    uint32_t play_one = id_list[i];
                    uint32_t play_two = id_list[i+1];
                
                    //匹配成功
                    hp->GamePrepare(play_one, play_two);
                }
               hp->MatchPoolClear(last);
               hp->UnlockMatchPool();

            //两两匹配
            }
        }


        string GetPlayerBoard(uint32_t &room_id)
        {
            string board;
            //uint32_t room_id = pm.GetPlayerRoomId(id);
            rm.GetBoard(room_id, board);
            return board;
        }

        uint32_t GetPlayerRoomId(uint32_t &id)
        {
            return pm.GetPlayerRoomId(id);
        }

        char GetPlayerPiece(uint32_t &room_id, uint32_t &id)
        {
            return rm.GetPlayerPiece(room_id, id);
        }

        bool IsMyTurn(uint32_t &room_id, uint32_t &id)
        {
            return rm.IsMyTurn(room_id, id);
        }

        void Step(uint32_t &room_id, uint32_t &id, int &x, int &y)
        {
            return rm.Step(room_id, id, x, y);
        }

        char Judge(uint32_t &room_id, uint32_t &id)
        {
            return rm.Judge(room_id, id);
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
