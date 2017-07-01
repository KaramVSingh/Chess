#if !defined(MOVE_H)
#define MOVE_H

#include "board.h"

#define MAX_MOVES 220

typedef enum m_type{
  NONE,
  STANDARD,
  ENPASSANT,
  CASTLE,
  PROMOTION
} m_type;

typedef struct move{
  char moved;
  char taken;
  int src_row;
  int src_col;
  int dst_row;
  int dst_col;
  m_type type;
  int color;
  float value;
  struct move *children;
  int length;
}move_t;

move_t *enpassant;
move_t *castle;


move_t *generate_moves(board_t *board, int color, int *length);

void print_move(move_t move);
void move_piece(board_t *board, move_t move, int color);
void undo_move(board_t *board, move_t move, int color);

void cardinal_moves(board_t *board, piece_t piece, move_t *moves, int *index);
void diagonal_moves(board_t *board, piece_t piece, move_t *moves, int *index);

move_t create_move(char moved, char taken, int src_row, int src_col, int dst_row, int dst_col, m_type type, int color);

#endif
