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
    cout << "Please Enter Your NickName: ";
    cin >> name;
    cout << "Please Enter Your Passwd: ";
    cin >> passwd;
    string passwd_; //保存第一次输入的密码
    cout << "Please Enter Your Passwd Again: ";
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

uint32_t Login()
{
    return 0;
}

