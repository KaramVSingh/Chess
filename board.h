#if !defined(BOARD_H)
#define BOARD_H

#include <stdio.h>

typedef struct{
  char name;
  int row;
  int col;
  int val;
  int taken;
  int has_moved;
  int color;
} piece_t;

struct move_history;

typedef struct{
  char board[8][8];
  piece_t pieces[2][16];
  struct move_history* head;
} board_t;

board_t * create_board();

void update_board(board_t *board);
void draw(board_t *board, FILE *stream);

int check(board_t *board, int color);

int place_piece(board_t *board, char name, int row, int col);

int is_draw(board_t *board);
#endif
