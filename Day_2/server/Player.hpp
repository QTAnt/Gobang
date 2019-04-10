#pragma once

#include <iostream>
#include <string>

using namespace std;

typedef enum{
    OFFLINE,
    ONLINE,
    MATCHING,
    PLAYING,
}status_t;

class Player{
    private:
        string name;
        string passwd;
        uint32_t id;

        int win;
        int lose;
        int tie; //平局
        
        status_t st;//状态

    public:
        Player(string &name, string &passwd, uint32_t id)
            :name(name_),passwd(passwd_),id(id_)
        {
            win = 0;
            lose = 0;
            tie = 0;
            status = OFFLINE;
        }

        int Rate()  //胜率
        {
            int total = win + lose;
            if(total == 0){
                return 0;
            }
            return win*100/total;
        }

        ~Player()
        {

        }
}

























