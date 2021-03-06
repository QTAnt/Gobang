#include <iostream>
#include <unordered_map>
#include "Room.hpp"

#define ROOM_ID 1024

class RoomManager{
    private:
        unordered_map<uint32_t, Room> room_set;
        uint32_t assign_id;  //分配房间号
    public:
        RoomManager():assign_id(ROOM_ID)
        {

        }

        uint32_t CreateRoom(uint32_t &one, uint32_t &two)
        {
            Room r(one, two);
            uint32_t id = assign_id++;
            room_set.insert({id, r});
            return id;
        }
        
        void GetBoard(uint32_t &room_id, string &_board)
        {
            room_set[room_id].Board(_board);
        }
        
        char GetPlayerPiece(uint32_t &room_id, uint32_t &id)
        {
            char st = room_set[room_id].Piece(id);
        }

        bool IsMyTurn(uint32_t &room_id, uint32_t &id)
        {
            bool is_my_turn = room_set[room_id].IsMyTurn(id);
            return is_my_turn;
        }

        void Step(uint32_t &room_id, uint32_t &id, int &x, int &y)
        {
            room_set[room_id].Step(id, x, y);
        }

        char Judge(uint32_t &room_id, uint32_t &id)
        {
            return room_set[room_id].GameResult(id);
        }

        ~RoomManager()
        {
            
        }

};
