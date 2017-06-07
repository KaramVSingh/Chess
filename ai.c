#include "ai.h"
#include <stdio.h>
#include <stdlib.h>

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

}

int make_move(board_t *board, int color){
  int i, row, col;

  for(i = 0; i < 16; i++){
    if(!board->pieces[color][i].taken){
      row = board->pieces[color][i].row;
      col = board->pieces[color][i].col;
      switch(board->pieces[color][i].name){
        /*
          might want a comprehensive move function here that can handle taking pieces and updating the
          board if necessary. Still need to be able to undo it though, maybe store previous moves in a
          linked list (or maybe just one move rather than all the moves) and have move and undo functions
          that handle that? Could be a useful thing to have in general for check and for the move function
          for the player too.
        */
        //temp assignment so that the program compiles
        board->board[row][col] = ' ';
      }
    }
  }

  return 0;
}
