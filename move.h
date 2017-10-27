#if !defined(MOVE_H)
#define MOVE_H

#include "board.h"

typedef enum m_type{
  NOMOVE,
  STANDARD,
  PREENPASSANT,
  ENPASSANT,
  CASTLE,
  PROMOTION
} m_type;

typedef struct move{
  piece_t *moved;
  piece_t *captured;
  char name;
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

typedef struct move_history{
  int index;
  move_t* move;
  struct move_history* next_move;
} move_history_t;

move_t enpassant;
move_t castle[2];


move_t *generate_moves(board_t *board, int color, int *length);

void print_move(move_t move);
void move_piece(board_t *board, move_t move, int color);
void undo_move(board_t *board, move_t move, int color);

void cardinal_moves(board_t *board, piece_t *piece, move_t *moves, int *index);
void diagonal_moves(board_t *board, piece_t *piece, move_t *moves, int *index);

move_t create_move(piece_t *moved, piece_t *taken, int src_row, int src_col, int dst_row, int dst_col, m_type type, int color);

void add_node(board_t* board, move_t move);
void print_history(board_t* board);
void remove_node(board_t* board);
move_t* get_node(board_t* board, int index);
int contains_node(board_t* board, move_t move);
int move_equals(move_t move1, move_t move2);

#endif
