#pragma once

#include "Player.hpp"
#include <unordered_map>

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
        player_set.insert(std::make_pair<uint32_t , Player>(id_, p));
        return id_;
    }

     ~PlayerManager()
    {

    }

}
