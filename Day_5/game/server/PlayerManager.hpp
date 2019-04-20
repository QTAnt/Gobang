#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>    //std::pair
#include "Log.hpp"  
#include "Player.hpp"

using namespace std;

#define DEAFULT_ID 10000

using namespace std;

class PlayerManager{
    private:
        std::unordered_map<uint32_t , Player>player_set;
        uint32_t assign_id;

    public:
        PlayerManager():assign_id(DEAFULT_ID)
    {

    }

    uint32_t InsertPlayer(string &name, string &passwd)
    {
        uint32_t id_ = assign_id++;

        Player p(name, passwd, id_); //定义一个用户p
        //player_set.insert(std::make_pair<uint32_t , Player>(id_, p));
        player_set.insert({id_, p});
        //cout << "insert player done..." << endl;
        LOG(INFO, "insert play done...");//日志——宏代码替换
        return id_;
    }

    uint32_t SearchPlayer(uint32_t &id, string &passwd)
    {
        auto iter = player_set.find(id);
        if(iter == player_set.end()){
            LOG(WARNING, "Search Player Error...");
            return 1;
        }
        Player &p = iter->second; // 拿到用户对应的second信息
        if(p.Passwd() != passwd){
            LOG(WARNING, "Player Passwd Error...");
            return 2;
        } 
        p.Online();
        LOG(INFO, "Player Online Success...");
        return id;
    }

    void SignalPlayer(uint32_t &one, uint32_t &two)
    {
        player_set[one].Signal();
        player_set[two].Signal();
    }

    void SetPlayStatus(uint32_t &one, uint32_t &two)
    {
        player_set[one].Playing();
        player_set[two].Playing();
    }

    void SetMatching(uint32_t &id)
    {
        player_set[id].Matching();
    }

    bool PlayerWait(uint32_t &id)
    {
        if(ETIMEDOUT == player_set[id].Wait()){//超时
            cout << "Play time out ..." << endl;
            return false;
        }
        cout << "Play wakeup..." << endl;
        return true;
    }

    int GetRate(uint32_t &id)
    {
        return player_set[id].Rate();
    }

    int Ready(uint32_t &id)
    {
        //return player_set[id].Status() == Playing ? true : false ;
        return player_set[id].Status();
    }

     ~PlayerManager()
    {

    }

};
