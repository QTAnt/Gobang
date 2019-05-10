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
        cout << "密码输入不一致!请重新输入！" << endl;
        return 1;
    }
    try{
		rpc_client client(ip, port);
        bool r = client.connect();
		if (!r) {
			std::cout << "网络开小差辣╥﹏╥..." << std::endl;
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
			std::cout << "网络开小差辣╥﹏╥..." << std::endl;
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
			std::cout << "网络开小差辣╥﹏╥..." << std::endl;
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
			std::cout << "网络开小差辣╥﹏╥..." << std::endl;
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
    int count = 20;  //匹配时间
    while(1)
    {
        int result = CheckReady(ip, port, id);
        if(result == 3){
            return true;
        }else if(result == 1){
            cout << "匹配失败！！" << endl;
            return false;
        }else{     //则状态为2
            printf("匹配中............... %2d\r", count--);
            fflush(stdout);//刷新
            if(count < 0){
                cout << endl;
                cout << "匹配超时!!" << endl; //匹配不上，从匹配池中拿走
                //remove client
                break;
            }
            sleep(1);
        }
        //cout << "check :" << result << endl;
        //sleep(1);
    }
    return false;
}

int GetBoard(string &ip, int &port, uint32_t &room_id, string &board)
{
   try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();       //短链，操作一次就断开链接
		if (!r) {
			std::cout << "网络开小差辣╥﹏╥..." << std::endl;
			return 3;
		}
        //cout << "Match id: " << id << endl;

		board = client.call<string>("RpcBoard", room_id);//根据房间号是最直接方便的，id与room_id有映射关系
    }
	catch (const std::exception& e){
		std::cout << e.what() << std::endl;
    }

}

uint32_t GetMyRoomId(string &ip, int &port, uint32_t &id)
{

   try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();       //短链，操作一次就断开链接
		if (!r) {
			std::cout << "网络开小差辣╥﹏╥..." << std::endl;
			return 3;
        }
        return client.call<uint32_t>("RpcPlayerRoomId", id);
   }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

char GetMyPiece(string &ip, int &port, uint32_t &room_id, uint32_t &id)
{

   try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();
        if (!r){
            std::cout << "网络开小差辣╥﹏╥..." << std::endl;
            return 3;
        }
        return client.call<char>("RpcPlayerPiece", room_id, id);
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

void ShowBoard(string &board)
{
    cout << "    ";
    for(auto i = 1; i <= 5; i++){
        cout << i << "   " ;
    }
    cout << endl;
    for(auto i = 0; i <= 5; i++){
        cout << "----";
    }
    cout << endl;

    //行列打印棋盘
    int size = board.size();
    int basic_size = 5;
    for(auto i = 0; i < size/basic_size; i++){
        cout << i+1
            << " |";
        for(auto j = 0; j < basic_size; j++){  //一行打完
            cout << " " << board[i*basic_size + j] << " |";  //board[]：下标
        }
        cout << endl;
        for(auto i = 0; i <= 5; i++){
            cout << "----";
        }
        cout << endl;

    }
}

bool IsMyTurn(string &ip, int &port, uint32_t room_id, uint32_t &id) 
{
   try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
            bool r = client.connect();
        if (!r){
            std::cout << "网络开小差辣╥﹏╥..." << std::endl;
            return 3;
        }
        return client.call<bool>("RpcIsMyTurn", room_id, id);
        }
        catch (const std::exception& e){
            std::cout << e.what() << std::endl;
        }
        return false;
}

bool PosIsRight(string &board, int x, int y)  //判定是否合法
{
    int pos = (x-1)*5 + (y-1);
    return board[pos] == ' ' ? true : false;
}

int Step(string &ip, int &port, uint32_t &room_id, uint32_t &id, int x, int y)
{
    
   try{
		rpc_client client(ip, port);     //调用时开辟，完了之后会被释放
        bool r = client.connect();
        if(!r){
            std::cout << "网络开小差辣╥﹏╥..." << std::endl;
            return 3;
        }
        client.call<void>("RpcStep", room_id, id, x-1, y-1);
        return 0;
   }
   catch (const std::exception& e){
       std::cout << e.what() << std::endl;
   }
}

char Judge(string &ip, int &port, uint32_t &room_id, uint32_t &id)   //服务器判断
{
    try{
        rpc_client client(ip, port);
        bool r = client.connect();
        if(!r){
            std::cout << "网络开小差辣╥﹏╥..." << std::endl;
            return 3;
        }
        return client.call<char>("RpcJudge", room_id, id);
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

void PlayGame(string &ip, int &port, uint32_t &id)
{
    int x, y;
    char result = 'N';
    string board;
    uint32_t room_id = GetMyRoomId(ip, port, id);
    cout << "room_id:" << room_id << endl;
    char piece = GetMyPiece(ip, port, room_id, id);
    cout << "piece:" << piece << endl;
    for(; ; ){
        system("clear");//清屏(更新棋盘)
        GetBoard(ip, port, room_id, board);
        ShowBoard(board);

        //自己下棋时判断一下当前游戏没有人赢，且棋盘未被沾满
        if((result = Judge(ip, port, room_id, id)) != 'N'){
            break;
        }

        if(!IsMyTurn(ip, port, room_id, id)){
             cout << "对方正在思考，请稍等..." << endl;
             sleep(1); //1秒钟检测一次，是否轮到自己下棋
             continue;
        }
            cout << "请输入你的落子位置：>" ;
            cin >> x >> y ; //判断棋子位置的合法性
            if(x >= 1 && x<= 5 && y >= 1 && y <= 5){
                if(!PosIsRight(board, x, y)){        //不用调用远端
                    cout << "你输入的位置已被占用，请重新输入T^T！" << endl;
                }else{
                    Step(ip, port, room_id, id, x, y);
                    result = Judge(ip, port, room_id, id);//走完之后判定输赢，Judge服务器判断，也是一个rpc
                    if(result != 'N'){
                        break;
                    }
                }
            }else{
                cout << "你输入的位置有误，请重新输入T^T！" << endl;
            }
    }

    //判断输赢
    if(result == 'E'){
        cout << "平局，都挺好!^o^" << endl;
    }
    else if(result == piece){
        cout << "恭喜你赢了！！^v^" << endl;
    }
    else{
        cout << "很遗憾，你输了...T_T" << endl;
    }
    //cout << "Match Success, Begin Game..." << endl;
  //  switch(result){
  //      case 'X':
  //          break;
  //      case 'O':
  //          break;
  //      case 'E':
  //          cout << "平局，再试一次趴"  << endl;
  //          break;
  //      default:
  //          break;
  //  }
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
                        PlayGame(ip, port, id);  //匹配成功，进入游戏
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

