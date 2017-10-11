#if !defined(AI_H)
#define AI_H
#include "move.h"
#include "board.h"

void init_ai();
int make_move(board_t *board,int difficulty, int color);


#endif
