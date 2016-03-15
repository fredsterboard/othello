#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <ctime>
#include "common.h"
#include "board.h"

using namespace std;

struct res_tuple
{
    int heuristic;
    int move;  
};

struct hash_value
{
    int lowerbound;
    int upperbound;
    int move;
};

class Player 
{
private:
    bool myside;
    unordered_map<hashkey, hash_value, KeyHasher> hashtable;

public:
    Board *myboard;

    Player(Side side);
    ~Player();

    hash_value *retrieved(Board *board, bool turn);
    int apply_transform(int move, int i);
    res_tuple *ABPruning(Board *board, int alpha, int beta, int depth, int le_depth, int turn);
    res_tuple *MTDF(int firstguess, int depth);
    int Deepening(int time_left);

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
