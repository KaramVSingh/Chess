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

void update_board(board_t *board){
  int i, j, k;

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
}
void draw(board_t * board){
  int i, j;
  char c;

  update_board(board);

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
  int i, j, k, row, col, done, shift;

  shift = color? 96: 64;
  if(check(board, color)){
    for(k = 0; k < 16; k++){
      row = board->pieces[color][k].row;
      col = board->pieces[color][k].col;
      switch (board->pieces[color][k].name) {
        case 'K':
        case 'k':
          for(i = -1; i < 2; i++){
            for(j = -1; j < 2; j++){
              board->pieces[color][k].row += i;
              board->pieces[color][k].col += j;
              if(board->pieces[color][k].row > 0 && board->pieces[color][k].col > 0 &&
                board->pieces[color][k].row < MAX_ROWS && board->pieces[color][k].col < MAX_COLS){
                  if((board->board[i][j] - shift) < 32 &&
                    !check(board, color)) return 1;;
              }
              board->pieces[color][k].row -= i;
              board->pieces[color][k].col -= j;
            }
          }
          break;
        case 'Q':
        case 'q':
          if(!board->pieces[color][k].taken){
            done = 0;
            while(!done){
              i = --board->pieces[color][k].row;
              j = board->pieces[color][k].col;
              if(i >= 0 && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            done = 0;
            //check down
            while(!done){
              i = ++board->pieces[color][k].row;
              j = board->pieces[color][k].col;
              if(i < MAX_ROWS && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            done = 0;
            //check left
            while(!done){
              i = board->pieces[color][k].row;
              j = --board->pieces[color][k].col;
              if(j >= 0 && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].col = col;
            done = 0;
            //check right
            while(!done){
              i = board->pieces[color][k].row;
              j = ++board->pieces[color][k].col;
              if(j < MAX_COLS && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].col = col;
          }
        case 'B':
        case 'b':
          if(!board->pieces[color][k].taken){
            done = 0;
            while(!done){
              i = --board->pieces[color][k].row;
              j = ++board->pieces[color][k].col;
              if(i >= 0 && j < MAX_COLS && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
            done = 0;
            //check up-left
            while(!done){
              i = --board->pieces[color][k].row;
              j = --board->pieces[color][k].col;
              if(i >= 0 && j >= 0 && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
            done = 0;
            //check down-left
            while(!done){
              i = ++board->pieces[color][k].row;
              j = --board->pieces[color][k].col;
              if(i < MAX_ROWS && j >= 0 && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
            done = 0;
            //check down-right
            while(!done){
              i = ++board->pieces[color][k].row;
              j = ++board->pieces[color][k].col;
              if(i < MAX_ROWS && j < MAX_COLS && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
          }
          break;
        case 'N':
        case 'n':
          if(!board->pieces[color][k].taken){
            i = board->pieces[color][k].row += 3;
            j = board->pieces[color][k].row += 2;
            if(i < MAX_ROWS && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row += 3;
            j = board->pieces[color][k].row -= 2;
            if(i < MAX_ROWS && j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 3;
            j = board->pieces[color][k].row += 2;
            if(i >= 0 && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 3;
            j = board->pieces[color][k].row -= 2;
            if(i >= 0&& j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row += 2;
            j = board->pieces[color][k].row += 3;
            if(i < MAX_ROWS && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 2;
            j = board->pieces[color][k].row += 3;
            if(i >= 0 && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row += 2;
            j = board->pieces[color][k].row -= 3;
            if(i < MAX_ROWS && j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 3;
            j = board->pieces[color][k].row -= 2;
            if(i >= 0 && j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)) return 1;
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
          }
          break;
        case 'R':
        case 'r':
          if(!board->pieces[color][k].taken){
            done = 0;
            //check up
            while(!done){
              i = --board->pieces[color][k].row;
              j = board->pieces[color][k].col;
              if(i >= 0 && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
            done = 0;
            //check left
            while(!done){
              i = board->pieces[color][k].row;
              j = --board->pieces[color][k].col;
              if(j >= 0 && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
            done = 0;
            //check down
            while(!done){
              i = ++board->pieces[color][k].row;
              j = board->pieces[color][k].col;
              if(i < MAX_ROWS && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;
            done = 0;
            //check down-right
            while(!done){
              i = board->pieces[color][k].row;
              j = ++board->pieces[color][k].col;
              if(j < MAX_COLS && (board->board[i][j] - shift) < 32){
                if(!check(board, color)) return 1;
              }else{
                done = 1;
              }
            }
          }
          break;
        case 'P':
        case 'p':
          if(!board->pieces[color][k].taken){
            i = ++board->pieces[color][k].row;
            j = board->pieces[color][k].col;
            if(i < MAX_ROWS && board->board[i][j] == 32 && !check(board, color))
              return 1;
            j = --board->pieces[color][k].col;
            if(j >= 0 && board->board[i][j] > 96 && !check(board, color))
              return 1;
            board->pieces[color][k].col += 2;
            j = board->pieces[color][k].col;
            if(j < MAX_COLS && board->board[i][j] > 96 && !check(board, color))
              return 1;
            board->pieces[color][k].row--;
            board->pieces[color][k].col--;
          }
          break;
      }
      board->pieces[color][k].row = row;
      board->pieces[color][k].col = col;
      update_board(board);
    }
    return -1;
  }
  return 0;
}

int check(board_t *board, int color){
  int left, right, up, down, i, j, k, done, shift;
  char c;

  left = right = up = down = TRUE;
  done = 0;
  shift = color? 32: 0;
  k = 1;
  update_board(board);

  i = board->pieces[color][0].row;
  j = board->pieces[color][0].col;
  //checking vertical and horizontal lines
  while(!done){
    if(up && i - k >= 0){
      c = board->board[i-k][j];
      if(c == ('Q' + shift) || c == ('R' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('B' + shift) || c == ('P' + shift)){
        up = FALSE;
      }else if(c != 32){
        up = FALSE;
      }
    }
    if(down && i + k < MAX_ROWS){
      c = board->board[i+k][j];
      if(c == ('Q' + shift) || c == ('R' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('B' + shift) || c == ('P' + shift)){
        down = FALSE;
      }else if(c != 32){
        down = FALSE;
      }
    }
    if(left && j - k >= 0){
      c = board->board[i][j-k];
      if(c == ('Q' + shift) || c == ('R' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('B' + shift) || c == ('P' + shift)){
        left = FALSE;
      }else if(c != 32){
        left = FALSE;
      }
    }
    if(right && j + k < MAX_COLS){
      c = board->board[i][j+k];
      if(c == ('Q' + shift) || c == ('R' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('B' + shift) || c == ('P' + shift)){
        right = FALSE;
      }else if(c != 32){
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
      if(c == ('Q' + shift) || c == ('B' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('R' + shift) || c == ('P' + shift)){
        up = FALSE;
      }else if(c != 32){
        up = FALSE;
      }
    }
    if(right && i+k < MAX_ROWS && j+k < MAX_COLS){
      c = board->board[i+k][j+k];
      if(c == ('Q' + shift) || c == ('B' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('R' + shift) || c == ('P' + shift)){
        right = FALSE;
      }else if(c != 32){
        right = FALSE;
      }
    }
    if(down && i+k < MAX_ROWS && j-k >= 0){
      c = board->board[i+k][j-k];
      if(c == ('Q' + shift) || c == ('B' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('R' + shift) || c == ('P' + shift)){
        down = FALSE;
      }else if(c != 32){
        down = FALSE;
      }
    }
    if(left && i-k >= 0 && j-k >= 0){
      c = board->board[i-k][j-k];
      if(c == ('Q' + shift) || c == ('B' + shift)){
        return TRUE;
      }else if(c == ('K' + shift) || c == ('N' + shift) || c == ('R' + shift) || c == ('P' + shift)){
        left = FALSE;
      }else if(c != 32){
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
  if(i+3 < MAX_ROWS && j+2 < MAX_COLS && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i+3 < MAX_ROWS && j-2 >= 0 && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i-3 >= 0 && j+2 < MAX_COLS && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i-3 >= 0 && j-2 >= 0 && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i+2 < MAX_ROWS && j+3 < MAX_COLS && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i+2 < MAX_ROWS && j-3 >= 0 && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i-2 >= 0 && j+3 < MAX_COLS && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;
  if(i-3 >= 0 && j-3 >= 0 && board->board[i+3][j+2] == ('K' + shift))
    return TRUE;

  return 0;
}

int place_piece(board_t *board, char name, int row, int col){
  int i = 0, color = BLACK;

  switch (name) {
    case 'K':
      color = WHITE;
    case 'k':
      i = 0;
      break;
    //TODO:should check if piece already taken for these, can use a pawn if need be
    case 'Q':
      color = WHITE;
    case 'q':
      i = 1;
      break;
    case 'B':
      color = WHITE;
    case 'b':
      if(board->pieces[color][2].taken){
        i = 2;
      }else if(board->pieces[color][3].taken){
        i = 3;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'N':
      color = WHITE;
    case 'n':
      if(board->pieces[color][4].taken){
        i = 4;
      }else if(board->pieces[color][5].taken){
        i = 5;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'R':
      color = WHITE;
    case 'r':
      if(board->pieces[color][6].taken){
        i = 6;
      }else if(board->pieces[color][7].taken){
        i = 7;
      }else{
      //replace with pawn stuff
        return 0;
      }
      break;
    case 'P':
      color = WHITE;
    case 'p':
      i = 8;
      while(i < 16 && !board->pieces[color][i].taken){
        i++;
      }
      if(i >= 16){
        //no more pawns
        return 0;
      }
      break;
  }
  board->pieces[color][i].taken = FALSE;
  board->pieces[color][i].row = row;
  board->pieces[color][i].col = col;
  return 1;
}
