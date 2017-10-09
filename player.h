#include "board.h"

//player h file

typedef struct player_t {
  int is_human;
  int difficulty;
  int piece_type;
} player_t;

/*
I think the "move" function should return an int- 1 if everything worked ok,
0 if there are no valid moves, essentially checkmate. either that or it can
be void and the check can be delegated to check, but I think this is better
because it prevents a move being made by the second player if the first can't
move
*/
int move(player_t* player, board_t *board);
