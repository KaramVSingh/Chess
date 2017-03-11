#include <stdio.h>
#include <stdlib.h>
#include "board.h"

board_t *create_board(){
  board_t *result;
  int i, j, k;

  result = (board_t *)malloc(sizeof(board_t));
  for(i = 0; i< 8; i++){
    result->white[i].row = 0;
    result->white[i+8].row = 1;
    result->white[i].taken = result->white[i+8].taken = 0;
    result->black[i].row = 7;
    result->black[i+8].row = 6;
    result->black[i].taken = result->black[i+8].taken = 0;

    result->white[i+8].name = 'P';
    result->white[i+8].col = i;
    result->black[i+8].name = 'p';
    result->black[i+8].col = i;
    result->white[i+8].val = 0;
    result->black[i+8].val = 0;
  }
  result->white[0].name = 'K';
  result->white[0].col = 4;
  result->white[0].val = 0;
  result->white[1].name = 'Q';
  result->white[1].col = 3;
  result->white[1].val = 0;
  result->white[2].name = 'B';
  result->white[2].col = 2;
  result->white[2].val = 0;
  result->white[3].name = 'B';
  result->white[3].col = 5;
  result->white[3].val = 0;
  result->white[4].name = 'N';
  result->white[4].col = 1;
  result->white[4].val = 0;
  result->white[5].name = 'N';
  result->white[5].col = 6;
  result->white[5].val = 0;
  result->white[6].name = 'R';
  result->white[6].col = 0;
  result->white[6].val = 0;
  result->white[7].name = 'R';
  result->white[7].col = 7;
  result->white[7].val = 0;
  result->black[0].name = 'k';
  result->black[0].col = 4;
  result->black[0].val = 0;
  result->black[1].name = 'q';
  result->black[1].col = 3;
  result->black[1].val = 0;
  result->black[2].name = 'b';
  result->black[2].col = 2;
  result->black[2].val = 0;
  result->black[3].name = 'b';
  result->black[3].col = 5;
  result->black[3].val = 0;
  result->black[4].name = 'n';
  result->black[4].col = 1;
  result->black[4].val = 0;
  result->black[5].name = 'n';
  result->black[5].col = 6;
  result->black[5].val = 0;
  result->black[6].name = 'r';
  result->black[6].col = 0;
  result->black[6].val = 0;
  result->black[7].name = 'r';
  result->black[7].col = 7;
  result->black[7].val = 0;

  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      result->board[i][j] = '\0';
      for(k = 0; k < 16; k++){
        if(result->white[k].row == i && result->white[k].col == j){
          result->board[i][j] = result->white[k].name;
        }else if(result->black[k].row == i && result->black[k].col == j){
          result->board[i][j] = result->black[k].name;
        }
      }
    }
  }
  //need to fill board with pieces
  /*
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
  */

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

int check(board_t *board, int color){
  int left, right, up, down, i, j, k, done;
  char c;

  left = right = up = down = TRUE;
  done = 0;
  k = 1;
  if(color == WHITE){
    i = board->white[0].row;
    j = board->white[0].col;
    //checking vertical and horizontal lines
    while(!done){
      if(up && i - k > 0){
        c = board->board[i-k][j];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96){
          up = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          up = FALSE;
        }
      }
      if(down && i + k < MAX_ROWS){
        c = board->board[i+k][j];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96){
          down = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          down = FALSE;
        }
      }
      if(left && j - k >= 0){
        c = board->board[i][j-k];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96){
          left = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          left = FALSE;
        }
      }
      if(right && j + k < MAX_COLS){
        c = board->board[i][j+k];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96){
          right = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          right = FALSE;
        }
      }
      k++;
      if(i-k < 0) up = FALSE;
      if(i+k == MAX_ROWS) down = FALSE;
      if(j-k < 0) left = FALSE;
      if(j+k == MAX_COLS) right = FALSE;

      if(!up && !down && !left && !right){
        done = TRUE;
      }
    }
  }else{
    i = board->black[0].row;
    j = board->black[0].col;
    //checking vertical and horizontal lines
    while(!done){
      if(up && i - k > 0){
        c = board->board[i-k][j];
        if(c == 'Q' || c == 'R'){
          return TRUE;
        }else if(c > 96){
          up = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'B' || c == 'P'){
          up = FALSE;
        }
      }
      if(down && i + k < MAX_ROWS){
        c = board->board[i+k][j];
        if(c == 'Q' || c == 'R'){
          return TRUE;
        }else if(c > 96){
          down = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'B' || c == 'P'){
          down = FALSE;
        }
      }
      if(left && j - k >= 0){
        c = board->board[i][j-k];
        if(c == 'Q' || c == 'R'){
          return TRUE;
        }else if(c > 96){
          left = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'B' || c == 'P'){
          left = FALSE;
        }
      }
      if(right && j + k < MAX_COLS){
        c = board->board[i][j+k];
        if(c == 'Q' || c == 'R'){
          return TRUE;
        }else if(c > 96){
          right = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'B' || c == 'P'){
          right = FALSE;
        }
      }
      k++;
      if(i-k < 0) up = FALSE;
      if(i+k == MAX_ROWS) down = FALSE;
      if(j-k < 0) left = FALSE;
      if(j+k == MAX_COLS) right = FALSE;

      if(!up && !down && !left && !right){
        done = TRUE;
      }
    }
  }

  return 0;
}
