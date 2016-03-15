#include "board.h"

using namespace std;

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() 
{
    black = BLACK_START;
    white = WHITE_START;
    moves[WHITE_TURN] = 0x80420100000;
    moves[BLACK_TURN] = 0x102004080000;
    total_pieces = 4;
}

/*
 * Destructor for the board.
 */
Board::~Board() 
{
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() 
{
    Board *newBoard = new Board();

    newBoard->black = black;
    newBoard->white = white;
    newBoard->moves[WHITE_TURN] = moves[WHITE_TURN];
    newBoard->moves[BLACK_TURN] = moves[BLACK_TURN];
    newBoard->total_pieces = total_pieces;

    return newBoard;
}

/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() 
{
    return !(moves[WHITE_TURN] || moves[BLACK_TURN]);
}

/*
 * modifies available moves bitboard.
 */
void Board::hasMoves(bool turn)
{
    uint64_t own = turn ? black : white;
    uint64_t opponent = turn ? white : black;
    uint64_t empty = ~((uint64_t) own | opponent);
    uint64_t potential;
    moves[turn] = (uint64_t) 0x0;

    //NORTH
    potential = ((own << 8) & NORTH) & opponent;
    while (potential)
    {
        potential = ((potential << 8) & NORTH);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //NORTH_EAST
    potential = ((own << 7) & NORTH_EAST) & opponent;
    while (potential)
    {
        potential = ((potential << 7) & NORTH_EAST);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //EAST
    potential = ((own >> 1) & EAST) & opponent;
    while (potential)
    {        
        potential = ((potential >> 1) & EAST);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //SOUTH_EAST
    potential = ((own >> 9) & SOUTH_EAST) & opponent;
    while (potential)
    {
        potential = ((potential >> 9) & SOUTH_EAST);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //SOUTH
    potential = ((own >> 8) & SOUTH) & opponent;
    while (potential)
    {
        potential = ((potential >> 8) & SOUTH);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //SOUTH_WEST
    potential = ((own >> 7) & SOUTH_WEST) & opponent;
    while (potential)
    {
        potential = ((potential >> 7) & SOUTH_WEST);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //WEST
    potential = ((own << 1) & WEST) & opponent;
    while (potential)
    {
        potential = ((potential << 1) & WEST);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }

    //NORTH_WEST
    potential = ((own << 9) & NORTH_WEST) & opponent;
    while (potential)
    {
        potential = ((potential << 9) & NORTH_WEST);
        moves[turn] = (potential & empty) | moves[turn];
        potential &= opponent;
    }
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(int input_move, bool turn)
{
    uint64_t own = turn ? black : white;
    uint64_t opponent = turn ? white : black;
    uint64_t move = ((uint64_t)1 << (63 - input_move));
    uint64_t result = move;
    uint64_t iterator1 = move;
    uint64_t iterator2 = move;
    uint64_t temp = move;

    //NORTH
    iterator2 = ((iterator2 << 8) & NORTH);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 << 8) & NORTH);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //NORTH_EAST
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 << 7) & NORTH_EAST);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 << 7) & NORTH_EAST);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //EAST
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 >> 1) & EAST);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 >> 1) & EAST);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //SOUTH_EAST
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 >> 9) & SOUTH_EAST);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 >> 9) & SOUTH_EAST);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //SOUTH
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 >> 8) & SOUTH);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 >> 8) & SOUTH);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //SOUTH_WEST
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 >> 7) & SOUTH_WEST);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 >> 7) & SOUTH_WEST);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //WEST
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 << 1) & WEST);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 << 1) & WEST);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //NORTH_WEST
    temp = move;
    iterator2 = move;
    iterator2 = ((iterator2 << 9) & NORTH_WEST);
    iterator1 = iterator2 & opponent;
    while (iterator1 != 0x0)
    {
        temp |= iterator1;
        iterator2 = ((iterator2 << 9) & NORTH_WEST);
        iterator1 = iterator2 & opponent;
    }
    if ((iterator2 & own) != 0x0)
    {
        result |= temp;
    }

    //UPDATE
    own |= result;
    opponent &= ~result;

    if (turn)
    {
        black = own;
        white = opponent;
    }
    else
    {
        white = own;
        black = opponent;
    }

    total_pieces++;

    hasMoves(BLACK_TURN);
    hasMoves(WHITE_TURN);
}

/*
 * Current count of given side's stones.
 */
int Board::count(bool turn) 
{
    uint64_t side = turn ? black : white;
    int count = 0;
    while (side) 
    {
        count++;
        side &= side - 1;
    }
    return count;
}

int Board::count_board(uint64_t board) 
{
    int count = 0;
    while (board) 
    {
        count++;
        board &= board - 1;
    }
    return count;
}

/*
 * Current count of given side's available moves.
 */
int Board::countMoves(bool turn) 
{
    uint64_t side = moves[turn];
    int count = 0;
    while (side) 
    {
        count++;
        side &= side - 1;
    }
    return count;
}

uint64_t Board::movesZero(bool turn)
{
    return (moves[turn] == 0x0);
}

/*
 * Heuristic for given board state.
 */
int Board::evaluate(bool turn)
{
    if (isDone())
    {
        return 32766 * (count(turn) > count(!turn) ? 1 : -1);
    }
    if (!count(!turn) || !countMoves(!turn))
        return 32766;
    uint64_t own = turn ? black : white;
    uint64_t opponent = turn ? white : black;
    int edge_count = count_board(own & 0xFF818181818181FF) - count_board(opponent & 0xFF818181818181FF);
    int corner_count = count_board(own & 0x8100000000000081) - count_board(opponent & 0x8100000000000081);
    int moves = countMoves(turn) - countMoves(!turn);
    int stones = count(turn) + count(!turn);
    return 3 * corner_count + edge_count + moves + stones;
}

uint64_t Board::board(bool turn)
{
    if (turn)
        return black;
    return white;
}

hashkey *Board::hash(int i, bool turn)
{
    hashkey *result;
    result = new hashkey;
    switch (i)
    {
        case 0:
            result->black = black;
            result->white = white;
            result->turn = turn;
            return result;
        case 1:
            result->black = vertiflip(black);
            result->white = vertiflip(white);
            result->turn = turn;
            return result;
        case 2:
            result->black = horiflip(black);
            result->white = horiflip(white);
            result->turn = turn;
            return result;
        case 3:
            result->black = diagflip(black);
            result->white = diagflip(white);
            result->turn = turn;
            return result;
        case 4:
            result->black = antidiagflip(black);
            result->white = antidiagflip(white);
            result->turn = turn;
            return result;
        case 5:
            result->black = horiflip(vertiflip(black));
            result->white = horiflip(vertiflip(white));
            result->turn = turn;
            return result;
        case 6:
            result->black = vertiflip(diagflip(black));
            result->white = vertiflip(diagflip(white));
            result->turn = turn;
            return result;
        default:
            result->black = diagflip(vertiflip(black));
            result->white = diagflip(vertiflip(white));
            result->turn = turn;
            return result;
    }
}

/*
 * hash helper functions.
 */
uint64_t Board::vertiflip(uint64_t board)
{
    const uint64_t constant1 = 0x00FF00FF00FF00FF;
    const uint64_t constant2 = 0x0000FFFF0000FFFF;
    board = ((board >>  8) & constant1) | ((board & constant1) <<  8);
    board = ((board >> 16) & constant2) | ((board & constant2) << 16);
    board = (board >> 32) | (board << 32);
    return board;
}

uint64_t Board::horiflip(uint64_t board) 
{
   const uint64_t constant1 = 0x5555555555555555;
   const uint64_t constant2 = 0x3333333333333333;
   const uint64_t constant3 = 0x0F0F0F0F0F0F0F0F;
   board = ((board >> 1) & constant1) +  2 * (board & constant1);
   board = ((board >> 2) & constant2) +  4 * (board & constant2);
   board = ((board >> 4) & constant3) + 16 * (board & constant3);
   return board;
}

uint64_t Board::diagflip(uint64_t board) 
{
   uint64_t temp;
   const uint64_t constant1 = 0x5500550055005500;
   const uint64_t constant2 = 0x3333000033330000;
   const uint64_t constant3 = 0x0f0f0f0f00000000;
   temp = constant3 & (board ^ (board << 28));
   board ^= temp ^ (temp >> 28);
   temp = constant2 & (board ^ (board << 14));
   board ^= temp ^ (temp >> 14);
   temp = constant1 & (board ^ (board << 7));
   board ^= temp ^ (temp >> 7);
   return board;
}

uint64_t Board::antidiagflip(uint64_t board) 
{
   uint64_t temp;
   const uint64_t constant1 = 0xaa00aa00aa00aa00;
   const uint64_t constant2 = 0xcccc0000cccc0000;
   const uint64_t constant3 = 0xf0f0f0f00f0f0f0f;
   temp = board ^ (board << 36);
   board ^= constant3 & (temp ^ (board >> 36));
   temp = constant2 & (board ^ (board << 18));
   board ^= temp ^ (temp >> 18);
   temp = constant1 & (board ^ (board << 9));
   board ^= temp ^ (temp >> 9);
   return board;
}

/*
 * Converts moves bitboard to int array.
 */
int *Board::getMoves(bool turn)
{
    uint64_t move_arr = moves[turn];
    int *result;
    int i = 63;
    int move_count = countMoves(turn);
    result = new int[move_count];
    while (move_count)
    {
        if (move_arr & 1)
        {
            result[move_count - 1] = i;
            move_count--;
        }
        move_arr >>= 1;
        i--;
    }
    return result;
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) 
{
    uint64_t iterator = 0x1;
    black = 0x0;
    white = 0x0;
    total_pieces = 0;
    for (int i = 63; i >= 0; i--) 
    {
        if (data[i] == 'b') 
        {
            black |= iterator;
            total_pieces++;
        } 
        if (data[i] == 'w') 
        {
            white |= iterator;
            total_pieces++;
        }
        iterator <<= 1;
    }
    hasMoves(BLACK_TURN);
    hasMoves(WHITE_TURN);
}

