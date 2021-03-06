#pragma once

#include <iostream>
#include <string>
#include <pthread.h>

#define TIME_OUT 20
using namespace std;

typedef enum{
    OFFLINE = 0,
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

        pthread_mutex_t lock;
        pthread_cond_t cond;
    public:
        Player()
        {

        }
        Player(string &name_, string &passwd_, uint32_t &id_)
            :name(name_),passwd(passwd_),id(id_)
        {
            win = 0;
            lose = 0;
            tie = 0;
            st = OFFLINE;
            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&cond, NULL);
        }

        const string& Passwd()
        {
            return passwd;
        }

        void Online()
        {
            st = ONLINE;
        }

        void Matching()
        {
            st = MATCHING;
        }

        void Playing()
        {
            st = PLAYING;
        }

        int Status()
        {
            return st;
        }
        int Rate()  //胜率
        {
            int total = win + lose;
            if(total == 0){
                return 0;
            }
            return win*100/total;
        }

        int Wait()
        {
               struct timespec ts;
               clock_gettime(CLOCK_REALTIME, &ts);
               ts.tv_sec += TIME_OUT;

            return pthread_cond_timedwait(&cond, &lock, &ts);
        }

        void Signal()
        {
            pthread_cond_signal(&cond);
        }
        ~Player()
        {
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&cond);
        }
};

























