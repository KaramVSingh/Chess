#include "ai.h"
#include <stdlib.h>
#include <stdio.h>
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

/*
  right now this generates the array of available moves (mostly not return_val
  moves yet) and then randomly selects one
*/
int make_move(board_t *board, int color){
  int i;
  move_t parent, move;

  parent.children = generate_moves(board, color, &parent.length);
  /*
  printf("Number of moves: %d\n", parent.length);
  for(i = 0; i < parent.length; i++){
    print_move(parent.children[i]);
  }
  */
  i = rand() % parent.length;
  //alphabeta(board, &parent, TRUE, 0, -1000, 1000);
  move = parent.children[i];
  move_piece(board, move, color);
  draw(board);
  print_move(move);
  return 0;
}

/*
  generates an array of all available moves for a given color with the
  board currenlty as it is
  TODO: special moves (pawns, castling), and disallow moves that place ai in check
*/
