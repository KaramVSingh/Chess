#include <stdio.h>
#include <stdlib.h>
#include "board.h"

board_t *create_board(){
  board_t *result;
  int i, j;

  result = (board_t *)malloc(sizeof(board_t));
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      result->board[i][j] = '\0';
    }
  }
  //need to fill board with pieces
  result->board[0][0] = result->board[0][7] = 'R';
  result->board[0][1] = result->board[0][6] = 'N';
  result->board[0][2] = result->board[0][5] = 'B';
  result->board[0][3] = 'K';//double check this
  result->board[0][4] = 'Q';
  result->board[7][0] = result->board[7][7] = 'r';
  result->board[7][1] = result->board[7][6] = 'n';
  result->board[7][2] = result->board[7][5] = 'b';
  result->board[7][3] = 'k';//double check this
  result->board[7][4] = 'q';
  for(i = 0; i < MAX_COLS; i++){
    result->board[1][i] = 'P';
    result->board[6][i] = 'p';
  }

  return result;
}

void draw(board_t * board){
  int i, j;
  char c;
  printf(" ___ ___ ___ ___ ___ ___ ___ ___\n");
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      c = board->board[i][j];
      if(c){
        printf("| %c ",c);
      }else{
        printf("|   ");
      }
    }
    printf("| %d\n", 8-i);
    printf("|___|___|___|___|___|___|___|___|\n");

  }
  printf("  A   B   C   D   E   F   G   H \n");
}
