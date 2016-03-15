/* Frederico Krauss */

To make my AI tournament worthy, I implemented a slightly optimized AB pruning algorithm as well as a transposition table. What I think makes my algorithm unique is that it hashes all possible translations of the board, giving the transposition table 8 times the power in cutting off branches from the search-tree. I also made sure to use 2 bit boards to represent my board as to make moves and generate moves more efficiently, using bitwise operations to parallelize the search for moves.

