#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <cstdint>
#include <unordered_map>
#include "common.h"

#define WHITE_TURN 0
#define BLACK_TURN 1

#define BLACK_START 0x810000000 
#define WHITE_START 0x1008000000

#define NORTH 0xFFFFFFFFFFFFFF00
#define NORTH_EAST 0x7F7F7F7F7F7F7F00
#define EAST 0x7F7F7F7F7F7F7F7F
#define SOUTH_EAST 0x007F7F7F7F7F7F7F
#define SOUTH 0x00FFFFFFFFFFFFFF
#define SOUTH_WEST 0x00FEFEFEFEFEFEFE
#define WEST 0xFEFEFEFEFEFEFEFE
#define NORTH_WEST 0xFEFEFEFEFEFEFE00

#define MAX_SEARCH_DEPTH 64

using namespace std;

struct hashkey
{
    uint64_t white;
    uint64_t black;
    bool turn;
    bool operator==(const hashkey &other) const
    { 
    return (white == other.white
            && black == other.black 
            && turn == other.turn);
    }
};

struct KeyHasher
{
  size_t operator()(const hashkey& k) const
  {
    return (((hash<int>()(k.white << 1))
             ^ (hash<int>()(k.black))) | k.turn);
  }
};

class Board {
   
private:
    uint64_t black;
    uint64_t white;
    uint64_t moves[2];   
      
public:
    int total_pieces;

    Board();
    ~Board();
    Board *copy();
        
    bool isDone();
    void hasMoves(bool turn);
    void doMove(int move, bool turn);
    int count(bool turn);
    int count_board(uint64_t board);
    int countMoves(bool turn);
    uint64_t movesZero(bool turn);
    int evaluate(bool turn);
    uint64_t board(bool turn);

    hashkey *hash(int i, bool turn);
    uint64_t vertiflip(uint64_t board);
    uint64_t horiflip(uint64_t board);
    uint64_t diagflip(uint64_t board);
    uint64_t antidiagflip(uint64_t board);

    int *getMoves(bool turn);

    void setBoard(char data[]);
};

#endif



