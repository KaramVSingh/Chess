#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float weights[8][8];

void init_ai(){
  int i, j;
  float x, y, val;

  for(i = 0; i < 8; i++){
    if(i < 4){
      x = (float) i;
    }else{
      x = -1.0*i + 7.0;
    }
    for(j = 0; j < 8; j++){
      if(j < 4){
        y = (float) j;
      }else{
        y = -1.0*j + 7.0;
      }
      val = -1.0 + (x + y)/3.0;
      weights[i][j] = val;
      //printf("%.2f\t", val);
    }
    //printf("\n");
  }
  srand(time(NULL));
}

int make_move(board_t *board, int color){
  int i;
  move_t parent, move;

  parent.children = generate_moves(board, color);
  i = rand() % MAX_MOVES;
  //alphabeta(board, &parent, TRUE, 0, -1000, 1000);
  move = parent.children[i];
  board->board[move.src_row][move.src_col] = ' ';
  board->board[move.dst_row][move.dst_col] = move.moved;
  if(move.taken != ' '){
    for(i = 0; i < 16; i++){
      if(board->pieces[!color][i].row == move.dst_row &&
        board->pieces[!color][i].col == move.dst_col &&
        board->pieces[!color][i].name == move.taken){
          board->pieces[!color][i].taken = TRUE;
        }
    }
  }
  print_move(move);
  return 0;
}

move_t *generate_moves(board_t *board, int color){
  move_t *moves;
  int i, j;

  moves = malloc(MAX_MOVES*sizeof(move_t));
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      moves[8*i + j].moved = 'Q';
      moves[8*i + j].taken = 'k';
      moves[8*i + j].src_row = i;
      moves[8*i + j].src_col = j;
      moves[8*i + j].dst_row = i;
      moves[8*i + j].dst_col = j;
    }
  }

  return moves;
}

void print_move(move_t move){
  printf("Piece %c moves from %c%d to %c%d", move.moved, (char)(move.src_col + 64), 8-move.src_row,
    (char)(move.dst_col + 64), 8-move.dst_row);
  if(move.taken != ' '){
    printf(" taking piece %c\n", move.taken);
  }else{
    printf("\n");
  }
}
