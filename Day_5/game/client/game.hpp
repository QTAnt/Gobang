#pragma once

#include <iostream>
#include <rpc_client.hpp>
#include <chrono>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "codec.h"

using namespace std;
using namespace std::chrono_literals;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;

uint32_t Register(const string &ip, const int &port,\
        string &name, string &passwd, uint32_t &id)
{
    cout << "请输入昵称: ";
    cin >> name;
    cout << "请设置密码";
    cin >> passwd;
    string passwd_; //保存第一次输入的密码
    cout << "确认密码: ";
    cin >> passwd_;

    if(passwd != passwd_){
        cout << "Inconsisten Passwd Input!" << endl;
        return 1;
    }
    try{
		rpc_client client(ip, port);
        bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}

		id = client.call<uint32_t>("RpcRegister", name, passwd);
    }
	catch (const std::exception& e){
		std::cout << e.what() << std::endl;
    }

    return 0;
}

uint32_t Login(const string &ip, const int &port)
{    
    uint32_t id;
    string passwd;

    cout << "请输入登录ID：";
    cin >> id;
    cout << "请输入密码：";
    cin >> passwd;

    uint32_t result = 0;
    try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();       //短链，操作一次就断开链接
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

		result = client.call<uint32_t>("RpcLogin", id, passwd);
    }
	catch (const std::exception& e){
		std::cout << e.what() << std::endl;
    }

    return result;
}
bool PushMatchPool(string &ip, int &port, uint32_t &id)
{
    try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();       //短链，操作一次就断开链接
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}
        //cout << "Match id: " << id << endl;

		return client.call<bool>("RpcMatchAndWait", id);
    }
	catch (const std::exception& e){
		std::cout << e.what() << std::endl;
    }
}

int CheckReady(string &ip, int &port, uint32_t &id)
{
    try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();       //短链，操作一次就断开链接
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}
        //cout << "Match id: " << id << endl;

		return client.call<int>("RpcPlayerReady", id);
    }
	catch (const std::exception& e){
		std::cout << e.what() << std::endl;
    }
}

bool Match(string &ip, int &port, uint32_t &id)
{
    PushMatchPool(ip, port, id);
    //int count = 20;
    while(1)
    {
        int result = CheckReady(ip, port, id);
        if(result == 3){
            return true;
        }else if(result == 1){
            return false;
        }else{     //则状态为2
            sleep(1);
        }
        //cout << "check :" << result << endl;
        //sleep(1);
    }

}

void PlayGame()
{
    cout << "Match Success, Begin Game..." << endl;
}

void Game(string &ip, int &port, uint32_t &id)
{
    int select = 0;
    volatile int quit = 0;
    while(!quit)
    {
        cout << "*************************************" << endl;
        cout << "***** 1.匹配             2.退出 *****" << endl;
        cout << "*************************************" << endl;
        cout << "请选择：";
        cin >> select;
        switch(select){
            case 1:
                {
                    if(Match(ip, port, id)){
                        PlayGame();
                    }else{
                        cout << "匹配失败，请重试！T^T" << endl;
                    }
                }
                break;
            case 2:
                quit = true;
                break;
            default:
                cout << "选择有误，请重新选择！" << endl;
                break;
        }
    }
}

