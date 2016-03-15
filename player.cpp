/* Small change to player.cpp */

#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) 
{
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    myboard = new Board();
    myside = (side == BLACK);
}

/*
 * Destructor for the player.
 */
Player::~Player() 
{
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) 
{
    if (opponentsMove != NULL)
    {
        int x = opponentsMove->getX();
        int y = opponentsMove->getY();
        myboard->doMove((x + 8 * y), !myside);
    }

    if (myboard->movesZero(myside))
    {
        return NULL;
    }

    int *moves = myboard->getMoves(myside);
    int move;
    Move *result;


    if (myboard->countMoves(myside) == 1)
    {
        myboard->doMove(moves[0], myside);
        int x = moves[0] % 8;
        int y = moves[0] / 8;
        delete moves;
        result = new Move(x, y);
        return result;
    }

    move = Deepening(msLeft);
    myboard->doMove(move, myside);
    int x = move % 8;
    int y = move / 8;
    result = new Move(x, y);
    return result;
}

int Player::Deepening(int time_left)
{
    clock_t begin, end;
    double diff;
    begin = clock();

    int move;
    int full = myboard->total_pieces;
    int empty = MAX_SEARCH_DEPTH - full;

    int heuristic;

    res_tuple *firstguess;
    firstguess = new res_tuple;
    firstguess->heuristic = 0;

    if (testingMinimax)
    {
        heuristic = firstguess->heuristic;
        delete firstguess;
        hashtable.clear();
        firstguess = MTDF(heuristic, 2);
        move = firstguess->move;
        delete firstguess;
        return move;   
    }
    for (int depth = 1; depth < empty; depth++)
    {
        heuristic = firstguess->heuristic;
        delete firstguess;
        hashtable.clear();
        firstguess = MTDF(heuristic, depth);
        end = clock();
        diff = time_left / 30000 - (end - begin) / CLOCKS_PER_SEC;
        if (diff < 5)
            break;
    }
    move = firstguess->move;
    delete firstguess;
    return move;
}

res_tuple *Player::MTDF(int firstguess, int depth)
{
    res_tuple *good;
    good = new res_tuple;
    good->heuristic = firstguess;
    int upperbound = 32767; 
    int lowerbound = -32767;
    int beta;
    while (lowerbound < upperbound)
    {
        if (good->heuristic == lowerbound)
            beta = good->heuristic + 1;
        else
            beta = good->heuristic;
        delete good;
        good = ABPruning(myboard->copy(), beta - 1, beta, depth, 0, myside);
        if (good->heuristic < beta)
            upperbound = good->heuristic;
        else
            lowerbound = good->heuristic;
    }
    return good;
}

int Player::apply_transform(int move, int i)
{
    int result = 63;
    uint64_t board = ((uint64_t)0x1 << (uint64_t)(63 - move));
    switch (i)
    {
        case 0:
            return move;
        case 1:
            board = myboard->vertiflip(board);
            break;
        case 2:
            board = myboard->horiflip(board);
            break;
        case 3:
            board = myboard->diagflip(board);
            break;
        case 4:
            board = myboard->antidiagflip(board);
            break;
        case 5:
            board = myboard->horiflip(myboard->vertiflip(board));
            break;
        case 6:
            board = myboard->diagflip(myboard->vertiflip(board));
            break;
        default:
            board = myboard->vertiflip(myboard->diagflip(board));
            break;
    }

    while(1)
    {
        if (board & 1)
        {
            return result;
        }
        board >>= 1;
        result--;  
    }
}

hash_value *Player::retrieved(Board *board, bool turn)
{
    hash_value *result;
    hashkey *boardhash;
    hashkey valid;
    for(int i = 0; i < 8; i++)
    {
        boardhash = board->hash(i, turn);
        if (hashtable.count(*boardhash) > 0)
        {
            valid = *boardhash;
            delete boardhash;
            result = &hashtable[valid];
            result->move = apply_transform(result->move, i);
            return result;
        }
        delete boardhash;
    }
    return NULL;
}

res_tuple *Player::ABPruning(Board *board, int alpha, int beta, int depth, int le_depth, int turn)
{
    Board *child;
    int a;
    int b;
    int *moves;
    int move_count;

    res_tuple *temp;
    temp = new res_tuple;

    res_tuple *result;
    result = new res_tuple;

    hash_value storage;
    storage.upperbound = 32767;
    storage.lowerbound = -32767;

    if (le_depth <= 9)
    {
        hash_value *retrieve;
        retrieve = retrieved(board, turn);

        if (retrieve != NULL)
        {
            if (retrieve->lowerbound >= beta)
            {
                result->heuristic = retrieve->lowerbound;
                result->move = retrieve->move;
                delete board;
                return result;
            }
            if (retrieve->upperbound <= alpha)
            {
                result->heuristic = retrieve->upperbound;
                result->move = retrieve->move;
                delete board;
                return result;
            }
            alpha = max(alpha, retrieve->lowerbound);
            beta = min(beta, retrieve->upperbound);
        }
    }

    if (depth == 0)
    {
        moves = board->getMoves(turn);
        result->heuristic = board->evaluate(myside);
        result->move = moves[0];
    }

    else if (turn == myside)
    {
        result->heuristic = -32767;
        a = alpha;
        moves = board->getMoves(turn);
        move_count = board->countMoves(turn);
        result->move = moves[0];
        for (int i = 0; i < move_count && result->heuristic < beta; i++)
        {
            child = board->copy();
            child->doMove(moves[i], turn);
            temp = ABPruning(child->copy(), a, beta, depth - 1, le_depth + 1, !turn);
            if (result->heuristic < temp->heuristic)
            {
                result->move = moves[i];
                result->heuristic = temp->heuristic;
            }
            delete temp;
            delete child;
            a = max(a, result->heuristic);
        }
    }

    else
    {
        result->heuristic = 32767; 
        b = beta;
        moves = board->getMoves(turn);
        move_count = board->countMoves(turn);
        result->move = moves[0];
        for (int i = 0; i < move_count && result->heuristic > alpha; i++)
        {
            child = board->copy();
            child->doMove(moves[i], turn);
            temp = ABPruning(child->copy(), alpha, b, depth - 1, le_depth + 1, !turn);
            if (result->heuristic > temp->heuristic)
            {
                result->move = moves[i];
                result->heuristic = temp->heuristic;
            }
            delete temp;
            delete child;
            b = min(b, result->heuristic);  
        }
    }

    if (le_depth <= 9)
    {
        storage.move = result->move;
        if (result->heuristic <= alpha)
            storage.upperbound = result->heuristic;
        if (result->heuristic > alpha && result->heuristic < beta)
        {
            storage.upperbound = result->heuristic;
            storage.lowerbound = result->heuristic;
        }
        if (result->heuristic >= beta)
            storage.lowerbound = result->heuristic;

        hashtable[*(board->hash(0, turn))] = storage;
    }

    delete board;
    return result;
}

