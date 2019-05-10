#pragma once

#include <iostream>
#include <string>

#define SIZE 5
#define BLACK 'X'
#define WHITE 'O'

using namespace std;

class Room{
    private:
        int one;  //'X'
        int two;  //'O'
        char piece[2];
        uint32_t current; //当前该谁下棋了
        char board[SIZE][SIZE];
        char result; //X, O, E, N

        pthread_mutex_t lock;
    public:
        Room()
        {

        }

        Room(uint32_t &id1, uint32_t &id2)
            :one(id1)
            ,two(id2)
        {
            piece[0] = 'X';
            piece[1] = 'O';
            memset(board, ' ', sizeof(board));
            result = 'N';
            current = one;
            pthread_mutex_init(&lock, NULL);
        }

        void Board(string &_board)//string _board是一个临时字符串，返回时有拷贝，在外部定义一下再引用,便于传输
        {
            //string _board;
            for(auto i = 0; i < SIZE; i++)
            {
                for(auto j = 0; j < SIZE; j++){
                    _board.push_back(board[i][j]);
                }
            }
            //return _board;
        }

        char Piece(uint32_t &id)
        {
            int pos = ( id == one ? 0 : 1);
            return piece[pos];
        }

        bool IsMyTurn(uint32_t &id)
        {
            return id == current ? true : false;
        }

        void Step(uint32_t &id, int &x, int &y)
        {
            if(current == id){
                int pos = (id == one ? 0 : 1); //piece[pos];
                    board[x][y] = piece[pos];
                    current = (id == one ? two : one); //切换下棋的一方
                    result = Judge();
            }
        }

        char GameResult(uint32_t &id)
        {
            return result;
        }

        char Judge()
        {
            int row = SIZE;
            int col = SIZE;
            for(auto i = 0; i < row; i++){
                if(board[i][0] != ' ' &&\
                   board[i][0] == board[i][1] && \
                   board[i][1] == board[i][2] && \
                   board[i][2] == board[i][3] && \
                   board[i][3] == board[i][4]){
                    return board[i][0];   //i行5个一样的棋子
                }
            }

            for(auto i = 0; i < col; i++){
                if(board[0][i] != ' ' &&\
                   board[0][i] == board[1][i] &&\
                   board[1][i] == board[2][i] &&\
                   board[2][i] == board[3][i] &&\
                   board[3][i] == board[4][i]){
                    return board[0][i];   //i列上都一样
                }
            }
            //对角线

            if(board[0][0] != ' ' &&\
               board[0][0] == board[1][1] &&\
               board[1][1] == board[2][2] &&\
               board[2][2] == board[3][3] &&\
               board[3][3] == board[4][4]){
                return board[0][0];
            }

            if(board[0][4] != ' ' &&\
               board[0][4] == board[1][3] &&\
               board[1][3] == board[2][2] &&\
               board[2][2] == board[3][1] &&\
               board[3][1] == board[4][0]){
                return board[0][4];
        }
            for(auto i = 0; i < row; i++){
                for(auto j = 0; j < col; j++){
                    if(board[i][j] == ' '){   //有空格，表明还未结束，继续游戏
                        return 'N';
                    }
                }
            }
            return 'E';   //棋盘满格，没有人赢，平局
     }

        ~Room()
        {
            pthread_mutex_destroy(&lock);
        }

};
