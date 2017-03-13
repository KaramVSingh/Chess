#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int check(board_t *board, int color);

board_t *create_board(){
  board_t *result;
  int i, j, k;

  result = (board_t *)malloc(sizeof(board_t));
  for(i = 0; i< 8; i++){
    result->pieces[WHITE][i].row = 0;
    result->pieces[WHITE][i+8].row = 1;
    result->pieces[WHITE][i].taken = result->pieces[WHITE][i+8].taken = 0;
    result->pieces[BLACK][i].row = 7;
    result->pieces[BLACK][i+8].row = 6;
    result->pieces[BLACK][i].taken = result->pieces[BLACK][i+8].taken = 0;

    result->pieces[WHITE][i+8].name = 'P';
    result->pieces[WHITE][i+8].col = i;
    result->pieces[BLACK][i+8].name = 'p';
    result->pieces[BLACK][i+8].col = i;
    result->pieces[WHITE][i+8].val = 1;
    result->pieces[BLACK][i+8].val = 1;
  }
  result->pieces[WHITE][0].name = 'K';
  result->pieces[WHITE][0].col = 4;
  result->pieces[WHITE][0].val = 0;
  result->pieces[WHITE][1].name = 'Q';
  result->pieces[WHITE][1].col = 3;
  result->pieces[WHITE][1].val = 9;
  result->pieces[WHITE][2].name = 'B';
  result->pieces[WHITE][2].col = 2;
  result->pieces[WHITE][2].val = 3;
  result->pieces[WHITE][3].name = 'B';
  result->pieces[WHITE][3].col = 5;
  result->pieces[WHITE][3].val = 3;
  result->pieces[WHITE][4].name = 'N';
  result->pieces[WHITE][4].col = 1;
  result->pieces[WHITE][4].val = 3;
  result->pieces[WHITE][5].name = 'N';
  result->pieces[WHITE][5].col = 6;
  result->pieces[WHITE][5].val = 3;
  result->pieces[WHITE][6].name = 'R';
  result->pieces[WHITE][6].col = 0;
  result->pieces[WHITE][6].val = 5;
  result->pieces[WHITE][7].name = 'R';
  result->pieces[WHITE][7].col = 7;
  result->pieces[WHITE][7].val = 5;
  result->pieces[BLACK][0].name = 'k';
  result->pieces[BLACK][0].col = 4;
  result->pieces[BLACK][0].val = 0;
  result->pieces[BLACK][1].name = 'q';
  result->pieces[BLACK][1].col = 3;
  result->pieces[BLACK][1].val = 9;
  result->pieces[BLACK][2].name = 'b';
  result->pieces[BLACK][2].col = 2;
  result->pieces[BLACK][2].val = 3;
  result->pieces[BLACK][3].name = 'b';
  result->pieces[BLACK][3].col = 5;
  result->pieces[BLACK][3].val = 3;
  result->pieces[BLACK][4].name = 'n';
  result->pieces[BLACK][4].col = 1;
  result->pieces[BLACK][4].val = 3;
  result->pieces[BLACK][5].name = 'n';
  result->pieces[BLACK][5].col = 6;
  result->pieces[BLACK][5].val = 3;
  result->pieces[BLACK][6].name = 'r';
  result->pieces[BLACK][6].col = 0;
  result->pieces[BLACK][6].val = 5;
  result->pieces[BLACK][7].name = 'r';
  result->pieces[BLACK][7].col = 7;
  result->pieces[BLACK][7].val = 5;

  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      result->board[i][j] = ' ';
    }
  }
  for(k = 0; k < 16; k++){
    i = result->pieces[WHITE][k].row;
    j = result->pieces[WHITE][k].col;
    result->board[i][j] = result->pieces[WHITE][k].name;
    i = result->pieces[BLACK][k].row;
    j = result->pieces[BLACK][k].col;
    result->board[i][j] = result->pieces[BLACK][k].name;
  }

  return result;
}

void draw(board_t * board){
  int i, j, k;
  char c;
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      board->board[i][j] = ' ';
    }
  }
  for(k = 0; k < 16; k++){
    i = board->pieces[WHITE][k].row;
    j = board->pieces[WHITE][k].col;
    if(!board->pieces[WHITE][k].taken) board->board[i][j] = board->pieces[WHITE][k].name;
    i = board->pieces[BLACK][k].row;
    j = board->pieces[BLACK][k].col;
    if(!board->pieces[BLACK][k].taken) board->board[i][j] = board->pieces[BLACK][k].name;
  }

  printf(" ___ ___ ___ ___ ___ ___ ___ ___\n");
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      c = board->board[i][j];
      printf("| %c ",c);
    }
    printf("| %d\n", 8-i);
    printf("|___|___|___|___|___|___|___|___|\n");

  }
  printf("  A   B   C   D   E   F   G   H \n");
}
/*
brute force check for a checkmate. This will check all the moves that the
pieces can make, seeing if the king is still in check for every move
*/
int check_status(board_t *board, int color){
  int i, j, k, row1, col1, done;
  board_t temp;

  if(check(board, color)){
    if(color == WHITE){
      //check if king has open moves
      for(i = -1; i < 2; i++){
        for(j = -1; j < 2; j++){
          temp.white[0].row += i;
          temp.white[0].col += j;
          if(temp.white[0].row > 0 && temp.white[0].col > 0 &&
            temp.white[0].row < MAX_ROWS && temp.white[0].col < MAX_COLS){
              if((temp.board[i][j] > 96 || temp.board[i][j] == 32) &&
                !check(&temp, color)) return 1;;
          }
          temp.white[0].row -= i;
          temp.white[0].col -= j;
        }
      }
      //check queen
      if(!temp.white[1].taken){
        done = 0;
        row1 = temp.white[1].row;
        col1 = temp.white[1].col;
        //check up
        while(!done){
          i = --temp.white[1].row;
          j = temp.white[1].col;
          if(i >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].row = row1;
        done = 0;
        //check down
        while(!done){
          i = ++temp.white[1].row;
          j = temp.white[1].col;
          if(i < MAX_ROWS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].row = row1;
        done = 0;
        //check left
        while(!done){
          i = temp.white[1].row;
          j = --temp.white[1].col;
          if(j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].col = col1;
        done = 0;
        //check right
        while(!done){
          i = temp.white[1].row;
          j = ++temp.white[1].col;
          if(j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].col = col1;
        done = 0;
        //check up-right
        while(!done){
          i = --temp.white[1].row;
          j = ++temp.white[1].col;
          if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].row = row1;
        temp.white[1].col = col1;
        done = 0;
        //check up-left
        while(!done){
          i = --temp.white[1].row;
          j = --temp.white[1].col;
          if(i >= 0 && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].row = row1;
        temp.white[1].col = col1;
        done = 0;
        //check down-left
        while(!done){
          i = ++temp.white[1].row;
          j = --temp.white[1].col;
          if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].row = row1;
        temp.white[1].col = col1;
        done = 0;
        //check down-right
        while(!done){
          i = ++temp.white[1].row;
          j = ++temp.white[1].col;
          if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.white[1].row = row1;
        temp.white[1].col = col1;
      }

      //check bishops
      for(k = 2; k < 4; k++){
        if(!temp.white[k].taken){
          row1 = temp.white[k].row;
          col1 = temp.white[k].col;

          done = 0;
          //check up-right
          while(!done){
            i = --temp.white[k].row;
            j = ++temp.white[k].col;
            if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
          done = 0;
          //check up-left
          while(!done){
            i = --temp.white[k].row;
            j = --temp.white[k].col;
            if(i >= 0 && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[1].row = row1;
          temp.white[1].col = col1;
          done = 0;
          //check down-left
          while(!done){
            i = ++temp.white[k].row;
            j = --temp.white[k].col;
            if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
          done = 0;
          //check down-right
          while(!done){
            i = ++temp.white[k].row;
            j = ++temp.white[k].col;
            if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
          done = 0;
        }
      }
      //check knights
      for(k = 4; k < 6; k++){
        if(!temp.white[k].taken){
          row1 = temp.white[k].row;
          col1 = temp.white[k].col;

          i = temp.white[k].row += 3;
          j = temp.white[k].row += 2;
          if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row += 3;
          j = temp.white[k].row -= 2;
          if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row -= 3;
          j = temp.white[k].row += 2;
          if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row -= 3;
          j = temp.white[k].row -= 2;
          if(i >= 0&& j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row += 2;
          j = temp.white[k].row += 3;
          if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row -= 2;
          j = temp.white[k].row += 3;
          if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row += 2;
          j = temp.white[k].row -= 3;
          if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;

          i = temp.white[k].row -= 3;
          j = temp.white[k].row -= 2;
          if(i >= 0 && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
        }
      }
      //check rooks
      for(k = 6; k < 8; k++){
        if(!temp.white[k].taken){
          row1 = temp.white[k].row;
          col1 = temp.white[k].col;
          done = 0;
          //check up
          while(!done){
            i = --temp.white[k].row;
            j = temp.white[k].col;
            if(i >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
          done = 0;
          //check left
          while(!done){
            i = temp.white[k].row;
            j = --temp.white[k].col;
            if(j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[1].row = row1;
          temp.white[1].col = col1;
          done = 0;
          //check down
          while(!done){
            i = ++temp.white[k].row;
            j = temp.white[k].col;
            if(i < MAX_ROWS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
          done = 0;
          //check down-right
          while(!done){
            i = temp.white[k].row;
            j = ++temp.white[k].col;
            if(j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.white[k].row = row1;
          temp.white[k].col = col1;
          done = 0;
        }
      }
      //check pawns
      for(k = 8; k < 16; k++){
        if(!temp.white[k].taken){
          i = ++temp.white[k].row;
          j = temp.white[k].col;
          if(i < MAX_ROWS && temp.board[i][j] == 32 && !check(&temp, color))
            return 1;
          j = --temp.white[k].col;
          if(j >= 0 && temp.board[i][j] > 96 && !check(&temp, color))
            return 1;
          temp.white[k].col += 2;
          j = temp.white[k].col;
          if(j < MAX_COLS && temp.board[i][j] > 96 && !check(&temp, color))
            return 1;
          temp.white[k].row--;
          temp.white[k].col--;
        }
      }
      return -1;
    }else{
      for(i = -1; i < 2; i++){
        for(j = -1; j < 2; j++){
          temp.black[0].row += i;
          temp.black[0].col += j;
          if(temp.black[0].row > 0 && temp.black[0].col > 0 &&
            temp.black[0].row < MAX_ROWS && temp.black[0].col < MAX_COLS){
              if((temp.board[i][j] > 96 || temp.board[i][j] == 32) &&
                !check(&temp, color)) return 1;;
          }
          temp.black[0].row -= i;
          temp.black[0].col -= j;
        }
      }
      //check queen
      if(!temp.black[1].taken){
        done = 0;
        row1 = temp.black[1].row;
        col1 = temp.black[1].col;
        //check up
        while(!done){
          i = --temp.black[1].row;
          j = temp.black[1].col;
          if(i >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].row = row1;
        done = 0;
        //check down
        while(!done){
          i = ++temp.black[1].row;
          j = temp.black[1].col;
          if(i < MAX_ROWS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].row = row1;
        done = 0;
        //check left
        while(!done){
          i = temp.black[1].row;
          j = --temp.black[1].col;
          if(j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].col = col1;
        done = 0;
        //check right
        while(!done){
          i = temp.black[1].row;
          j = ++temp.black[1].col;
          if(j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].col = col1;
        done = 0;
        //check up-right
        while(!done){
          i = --temp.black[1].row;
          j = ++temp.black[1].col;
          if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].row = row1;
        temp.black[1].col = col1;
        done = 0;
        //check up-left
        while(!done){
          i = --temp.black[1].row;
          j = --temp.black[1].col;
          if(i >= 0 && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].row = row1;
        temp.black[1].col = col1;
        done = 0;
        //check down-left
        while(!done){
          i = ++temp.black[1].row;
          j = --temp.black[1].col;
          if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].row = row1;
        temp.black[1].col = col1;
        done = 0;
        //check down-right
        while(!done){
          i = ++temp.black[1].row;
          j = ++temp.black[1].col;
          if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }else{
            done = 1;
          }
        }
        temp.black[1].row = row1;
        temp.black[1].col = col1;
      }

      //check bishops
      for(k = 2; k < 4; k++){
        if(!temp.black[k].taken){
          row1 = temp.black[k].row;
          col1 = temp.black[k].col;

          done = 0;
          //check up-right
          while(!done){
            i = --temp.black[k].row;
            j = ++temp.black[k].col;
            if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
          done = 0;
          //check up-left
          while(!done){
            i = --temp.black[k].row;
            j = --temp.black[k].col;
            if(i >= 0 && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[1].row = row1;
          temp.black[1].col = col1;
          done = 0;
          //check down-left
          while(!done){
            i = ++temp.black[k].row;
            j = --temp.black[k].col;
            if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
          done = 0;
          //check down-right
          while(!done){
            i = ++temp.black[k].row;
            j = ++temp.black[k].col;
            if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
          done = 0;
        }
      }
      //check knights
      for(k = 4; k < 6; k++){
        if(!temp.black[k].taken){
          row1 = temp.black[k].row;
          col1 = temp.black[k].col;

          i = temp.black[k].row += 3;
          j = temp.black[k].row += 2;
          if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row += 3;
          j = temp.black[k].row -= 2;
          if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row -= 3;
          j = temp.black[k].row += 2;
          if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row -= 3;
          j = temp.black[k].row -= 2;
          if(i >= 0&& j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row += 2;
          j = temp.black[k].row += 3;
          if(i < MAX_ROWS && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row -= 2;
          j = temp.black[k].row += 3;
          if(i >= 0 && j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row += 2;
          j = temp.black[k].row -= 3;
          if(i < MAX_ROWS && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;

          i = temp.black[k].row -= 3;
          j = temp.black[k].row -= 2;
          if(i >= 0 && j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
            if(!check(&temp, color)) return 1;
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
        }
      }
      //check rooks
      for(k = 6; k < 8; k++){
        if(!temp.black[k].taken){
          row1 = temp.black[k].row;
          col1 = temp.black[k].col;
          done = 0;
          //check up
          while(!done){
            i = --temp.black[k].row;
            j = temp.black[k].col;
            if(i >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
          done = 0;
          //check left
          while(!done){
            i = temp.black[k].row;
            j = --temp.black[k].col;
            if(j >= 0 && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[1].row = row1;
          temp.black[1].col = col1;
          done = 0;
          //check down
          while(!done){
            i = ++temp.black[k].row;
            j = temp.black[k].col;
            if(i < MAX_ROWS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
          done = 0;
          //check down-right
          while(!done){
            i = temp.black[k].row;
            j = ++temp.black[k].col;
            if(j < MAX_COLS && (temp.board[i][j] > 96 || temp.board[i][j] == 32)){
              if(!check(&temp, color)) return 1;
            }else{
              done = 1;
            }
          }
          temp.black[k].row = row1;
          temp.black[k].col = col1;
          done = 0;
        }
      }
      //check pawns
      for(k = 8; k < 16; k++){
        if(!temp.black[k].taken){
          i = --temp.black[k].row;
          j = temp.black[k].col;
          if(i >= 0 && temp.board[i][j] == 32 && !check(&temp, color))
            return 1;
          j = --temp.black[k].col;
          if(j >= 0 && temp.board[i][j] > 96 && !check(&temp, color))
            return 1;
          temp.black[k].col += 2;
          j = temp.black[k].col;
          if(j < MAX_COLS && temp.board[i][j] > 96 && !check(&temp, color))
            return 1;
          temp.black[k].row--;
          temp.black[k].col--;
        }
      }
      return -1;
    }
  }
  return 0;
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
      if(up && i - k >= 0){
        c = board->board[i-k][j];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96 && c != 32){
          up = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          up = FALSE;
        }
      }
      if(down && i + k < MAX_ROWS){
        c = board->board[i+k][j];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96 && c != 32){
          down = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          down = FALSE;
        }
      }
      if(left && j - k >= 0){
        c = board->board[i][j-k];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96 && c != 32){
          left = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          left = FALSE;
        }
      }
      if(right && j + k < MAX_COLS){
        c = board->board[i][j+k];
        if(c == 'q' || c == 'r'){
          return TRUE;
        }else if(c < 96 && c != 32){
          right = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'b' || c == 'p'){
          right = FALSE;
        }
      }
      k++;
      if(i-k < 0) up = FALSE;
      if(i+k >= MAX_ROWS) down = FALSE;
      if(j-k < 0) left = FALSE;
      if(j+k >= MAX_COLS) right = FALSE;

      if(!up && !down && !left && !right){
        done = TRUE;
      }
    }
    //checking diagonals
    /*
    I'm going to use the same variable names as before for simplicity's sake.
    I'll be going clockwise around, so up == up-right, right == down-right,
    down == down-left, and left == up-left.
    */
    left = right = up = down = TRUE;
    done = 0;
    k = 1;
    while(!done){
      if(up && i-k >= 0 && j+k < MAX_COLS){
        c = board->board[i-k][j+k];
        if(c == 'q' || c == 'b'){
          return TRUE;
        }else if(c < 96 && c != 32){
          up = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'r' || c == 'p'){
          up = FALSE;
        }
      }
      if(right && i+k < MAX_ROWS && j+k < MAX_COLS){
        c = board->board[i+k][j+k];
        if(c == 'q' || c == 'b'){
          return TRUE;
        }else if(c < 96 && c != 32){
          right = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'r' || c == 'p'){
          right = FALSE;
        }
      }
      if(down && i+k < MAX_ROWS && j-k >= 0){
        c = board->board[i+k][j-k];
        if(c == 'q' || c == 'b'){
          return TRUE;
        }else if(c < 96 && c != 32){
          down = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'r' || c == 'p'){
          down = FALSE;
        }
      }
      if(left && i-k >= 0 && j-k >= 0){
        c = board->board[i-k][j-k];
        if(c == 'q' || c == 'b'){
          return TRUE;
        }else if(c < 96 && c != 32){
          left = FALSE;
        }else if(c == 'k' || c == 'n' || c == 'r' || c == 'p'){
          left = FALSE;
        }
      }
      k++;
      if(i-k < 0 || j+k >= MAX_COLS) up = FALSE;
      if(i+k >= MAX_ROWS || j+k >= MAX_COLS) right = FALSE;
      if(i+k >= MAX_ROWS || j-k < 0) down = FALSE;
      if(i-k < 0 || j-k < 0) left = FALSE;

      if(!up && !down && !left && !right){
        done = TRUE;
      }
    }
    //checking for knights
    if(i+3 < MAX_ROWS && j+2 < MAX_COLS && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i+3 < MAX_ROWS && j-2 >= 0 && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i-3 >= 0 && j+2 < MAX_COLS && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i-3 >= 0 && j-2 >= 0 && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i+2 < MAX_ROWS && j+3 < MAX_COLS && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i+2 < MAX_ROWS && j-3 >= 0 && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i-2 >= 0 && j+3 < MAX_COLS && board->board[i+3][j+2] == 'k')
      return TRUE;
    if(i-3 >= 0 && j-3 >= 0 && board->board[i+3][j+2] == 'k')
      return TRUE;

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
      if(i+k >= MAX_ROWS) down = FALSE;
      if(j-k < 0) left = FALSE;
      if(j+k >= MAX_COLS) right = FALSE;

      if(!up && !down && !left && !right){
        done = TRUE;
      }
    }

    left = right = up = down = TRUE;
    done = 0;
    k = 1;
    while(!done){
      if(up && i-k >= 0 && j+k < MAX_COLS){
        c = board->board[i-k][j+k];
        if(c == 'Q' || c == 'B'){
          return TRUE;
        }else if(c > 96){
          up = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'R' || c == 'P'){
          up = FALSE;
        }
      }
      if(right && i+k < MAX_ROWS && j+k < MAX_COLS){
        c = board->board[i+k][j+k];
        if(c == 'Q' || c == 'B'){
          return TRUE;
        }else if(c > 96){
          right = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'R' || c == 'P'){
          right = FALSE;
        }
      }
      if(down && i+k < MAX_ROWS && j-k >= 0){
        c = board->board[i+k][j-k];
        if(c == 'Q' || c == 'B'){
          return TRUE;
        }else if(c > 96){
          down = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'R' || c == 'P'){
          down = FALSE;
        }
      }
      if(left && i-k >= 0 && j-k >= 0){
        c = board->board[i-k][j-k];
        if(c == 'Q' || c == 'B'){
          return TRUE;
        }else if(c > 96){
          left = FALSE;
        }else if(c == 'K' || c == 'N' || c == 'R' || c == 'P'){
          left = FALSE;
        }
      }
      k++;
      if(i-k < 0 || j+k >= MAX_COLS) up = FALSE;
      if(i+k >= MAX_ROWS || j+k >= MAX_COLS) right = FALSE;
      if(i+k >= MAX_ROWS || j-k < 0) down = FALSE;
      if(i-k < 0 || j-k < 0) left = FALSE;

      if(!up && !down && !left && !right){
        done = TRUE;
      }
    }

    if(i+3 < MAX_ROWS && j+2 < MAX_COLS && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i+3 < MAX_ROWS && j-2 >= 0 && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i-3 >= 0 && j+2 < MAX_COLS && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i-3 >= 0 && j-2 >= 0 && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i+2 < MAX_ROWS && j+3 < MAX_COLS && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i+2 < MAX_ROWS && j-3 >= 0 && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i-2 >= 0 && j+3 < MAX_COLS && board->board[i+3][j+2] == 'K')
      return TRUE;
    if(i-3 >= 0 && j-3 >= 0 && board->board[i+3][j+2] == 'K')
      return TRUE;

  }

  return 0;
}

int place_piece(board_t *board, char name, int row, int col){
  int i = 0;

  switch (name) {
    case 'k':
      board->black[0].taken = FALSE;
      board->black[0].row = row;
      board->black[0].col = col;
      break;
    //TODO:should check if piece already taken for these, can use a pawn if need be
    case 'q':
      board->black[1].taken = FALSE;
      board->black[1].row = row;
      board->black[1].col = col;
      break;
    case 'b':
      if(board->black[2].taken){
        board->black[2].taken = FALSE;
        board->black[2].row = row;
        board->black[2].col = col;
      }else if(board->black[3].taken){
        board->black[3].taken = FALSE;
        board->black[3].row = row;
        board->black[3].col = col;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'n':
      if(board->black[4].taken){
        board->black[4].taken = FALSE;
        board->black[4].row = row;
        board->black[4].col = col;
      }else if(board->black[5].taken){
        board->black[5].taken = FALSE;
        board->black[5].row = row;
        board->black[5].col = col;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'r':
      if(board->black[6].taken){
        board->black[6].taken = FALSE;
        board->black[6].row = row;
        board->black[6].col = col;
      }else if(board->black[7].taken){
        board->black[7].taken = FALSE;
        board->black[7].row = row;
        board->black[7].col = col;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'p':
      while(i < 8 && !board->black[8+i].taken){
        i++;
      }
      if(i < 8){
        board->black[0].taken = FALSE;
        board->black[0].row = row;
        board->black[0].col = col;
      }else{
        //no more pawns
        return 0;
      }
      break;
    case 'K':
      board->white[0].taken = FALSE;
      board->white[0].row = row;
      board->white[0].col = col;
      break;
    //TODO:should check if piece already taken for these, can use a pawn if need be
    case 'Q':
      board->white[1].taken = FALSE;
      board->white[1].row = row;
      board->white[1].col = col;
      break;
    case 'B':
      if(board->white[2].taken){
        board->white[2].taken = FALSE;
        board->white[2].row = row;
        board->white[2].col = col;
      }else if(board->white[3].taken){
        board->white[3].taken = FALSE;
        board->white[3].row = row;
        board->white[3].col = col;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'N':
      if(board->white[4].taken){
        board->white[4].taken = FALSE;
        board->white[4].row = row;
        board->white[4].col = col;
      }else if(board->white[5].taken){
        board->white[5].taken = FALSE;
        board->white[5].row = row;
        board->white[5].col = col;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'R':
      if(board->white[6].taken){
        board->white[6].taken = FALSE;
        board->white[6].row = row;
        board->white[6].col = col;
      }else if(board->white[7].taken){
        board->white[7].taken = FALSE;
        board->white[7].row = row;
        board->white[7].col = col;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'P':
      while(i < 8 && !board->white[8+i].taken){
        i++;
      }
      if(i < 8){
        board->white[0].taken = FALSE;
        board->white[0].row = row;
        board->white[0].col = col;
      }else{
        //no more pawns
        return 0;
      }
      break;
    default:
      return 0;
      break;
  }
  return 1;
}
