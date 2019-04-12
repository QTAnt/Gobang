#pragma once

#include <iostream>
#include "PlayerManager.hpp"
//#include "RoomManager.hpp"

using namespace std;

class Hall{
    private:
        PlayerManager pm;
        //RoomManager rm;

    public:
        Hall()
        {

        }

        uint32_t Register(string &name, string &passwd)
        {
            return pm.InsertPlayer(name, passwd);
        }

        uint32_t Login(uint32_t &id, string &passwd)
        {
            return pm.SearchPlayer(id, passwd);
        }

        ~Hall()
        {

        }
};
