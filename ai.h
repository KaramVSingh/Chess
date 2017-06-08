#if !defined(AI_H)
#define AI_H
#include "board.h"

#define MAX_MOVES 64

typedef struct move{
  char moved;
  char taken;
  int src_row;
  int src_col;
  int dst_row;
  int dst_col;
  int color;
  float value;
  struct move *children;
}move_t;

void init_ai();
int make_move(board_t *board, int color);
move_t *generate_moves(board_t *board, int color);
void alphabeta(board_t *board, move_t *move, int max, int depth, float alpha, float beta);

void print_move(move_t move);
void move_piece(board_t *board, move_t move, int color);
void undo_move(board_t *board, move_t move, int color);

move_t create_move(char moved, char taken, int src_row, int src_col, int dst_row, int dst_col, int color);

float calculate_move(board_t *board, move);

#endif
