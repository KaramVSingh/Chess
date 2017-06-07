#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int check(board_t *board, int color);

board_t *create_board(){
  board_t *result;
  piece_t board[8][8];
  int i, j, k;
  int values[] = {5, 3, 3, 9, 1000, 3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1};
  int cols[] = {4, 3, 2, 5, 1, 6, 0, 7, 0, 1, 2, 3, 4, 5, 6, 7};
  char names[] = {'R', 'N', 'B', 'Q','K', 'B', 'N', 'R', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};

  result = (board_t *)malloc(sizeof(board_t));
  result->board = (piece_t *)malloc(64*sizeof(piece_t));
  result->pieces = (piece_t *)malloc(16*sizeof(piece_t));

  for(i = 0; i < 2; i++){
    for(j = 0; j < 8; j++){
      result->board[i][j].name = names[j + i*8] + 32;
      result->board[i][j].val = values[j + i*8];
      result->board[i][j].taken = 0;
      result->board[i][j].color = BLACK;
      result->board[i][j].has_moved = 0;
      result->board[i][j].threatened = 0;

      result->board[7-i][j].name = names[j + i*8];
      result->board[7-i][j].val = values[j + i*8];
      result->board[7-i][j].taken = 0;
      result->board[7-i][j].color = WHITE;
      result->board[7-i][j].has_moved = 0;
      result->board[7-i][j].threatened = 0;
    }
  }

  for(i = 2; i < 6; i++){
    for(j = 0; j < 8; j++){
      result->board[i][j].name = ' ';
      result->board[i][j].val = 0;
      result->board[i][j].taken = 0;
      result->board[i][j].color = EMPTY;
      result->board[i][j].has_moved = 0;
      result->board[i][j].threatened = 0;
    }
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
  int i, j, white, black, white_done, black_done;
  char c;

  update_board(board);

  black = white = 0;
  printf(" ___ ___ ___ ___ ___ ___ ___ ___");
  printf("      White   Black\n");
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      c = board->board[i][j];
      printf("| %c ",c);
    }
    printf("| %d", 8-i);
    white_done = black_done = 0;

    while(white_done < 2 && white < 16){
      if(board->pieces[WHITE][white].taken){
        white_done++;
        printf("   %c", board->pieces[WHITE][white].name);
      }
      white++;
    }
    for( ; white_done < 2; white_done++){
      printf("    ");
    }
    while(black_done < 2 && black < 16){
      if(board->pieces[BLACK][black].taken){
        black_done++;
        printf("   %c", board->pieces[BLACK][black].name);
      }
      black++;
    }
    printf("\n|___|___|___|___|___|___|___|___|\n");

  }
  printf("  A   B   C   D   E   F   G   H \n");
}
/*
brute force check for a checkmate. This will check all the moves that the
pieces can make, seeing if the king is still in check for every move
*/
int check_status(board_t *board, int color){
  int i, j, k, row, col, done, shift, return_val;

  shift = color? 96: 64;
  if(check(board, color)){
    for(k = 0; k < 16; k++){
      row = board->pieces[color][k].row;
      col = board->pieces[color][k].col;
      return_val = 0;
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
                    !check(board, color)){
                      return_val = CHECK;
                    }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)) {
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
              }else{
                done = 1;
              }
            }
          }
          break;
        case 'N':
        case 'n':
          if(!board->pieces[color][k].taken){
            i = board->pieces[color][k].row += 2;
            j = board->pieces[color][k].row += 1;
            if(i < MAX_ROWS && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row += 2;
            j = board->pieces[color][k].row -= 1;
            if(i < MAX_ROWS && j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 2;
            j = board->pieces[color][k].row += 1;
            if(i >= 0 && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 2;
            j = board->pieces[color][k].row -= 1;
            if(i >= 0&& j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row += 1;
            j = board->pieces[color][k].row += 2;
            if(i < MAX_ROWS && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row += 1;
            j = board->pieces[color][k].row -= 2;
            if(i >= 0 && j < MAX_COLS && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 1;
            j = board->pieces[color][k].row += 2;
            if(i < MAX_ROWS && j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
            }
            board->pieces[color][k].row = row;
            board->pieces[color][k].col = col;

            i = board->pieces[color][k].row -= 1;
            j = board->pieces[color][k].row -= 2;
            if(i >= 0 && j >= 0 && (board->board[i][j] - shift) < 32){
              if(!check(board, color)){
                return_val = CHECK;
              }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
                if(!check(board, color)){
                  return_val = CHECK;
                  done = 1;
                }
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
            if(i < MAX_ROWS && board->board[i][j] == 32 && !check(board, color)){
              return_val = CHECK;
            }
            j = --board->pieces[color][k].col;
            if(j >= 0 && board->board[i][j] > 96 && !check(board, color)){
              return_val = 1;
            }
            board->pieces[color][k].col += 2;
            j = board->pieces[color][k].col;
            if(j < MAX_COLS && board->board[i][j] > 96 && !check(board, color)){
              return_val = 1;
            }
            board->pieces[color][k].row--;
            board->pieces[color][k].col--;
          }
          break;
      }
      board->pieces[color][k].row = row;
      board->pieces[color][k].col = col;
      update_board(board);
      if(return_val){
        return CHECK;
      }
    }
    return -CHECKMATE;
  }
  return NO_CHECK;
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
  if(i+2 < MAX_ROWS && j+1 < MAX_COLS && board->board[i+2][j+1] == ('N' + shift))
    return TRUE;
  if(i+2 < MAX_ROWS && j-1 >= 0 && board->board[i+2][j-1] == ('N' + shift))
    return TRUE;
  if(i-2 >= 0 && j+1 < MAX_COLS && board->board[i-2][j+1] == ('N' + shift))
    return TRUE;
  if(i-2 >= 0 && j-1 >= 0 && board->board[i-2][j-1] == ('N' + shift))
    return TRUE;
  if(i+1 < MAX_ROWS && j+2 < MAX_COLS && board->board[i+1][j+2] == ('N' + shift))
    return TRUE;
  if(i+1 < MAX_ROWS && j-2 >= 0 && board->board[i+1][j-2] == ('N' + shift))
    return TRUE;
  if(i-1 >= 0 && j+2 < MAX_COLS && board->board[i-1][j+2] == ('N' + shift))
    return TRUE;
  if(i-1 >= 0 && j-2 >= 0 && board->board[i-1][j-2] == ('N' + shift))
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


int check_movement(board_t *board, int piece_type, int piece_number, int row, int col) {

  char type = board->pieces[piece_type][piece_number].name;

  switch (type) {

    case 'K' :
    case 'k' :if (abs(board->pieces[piece_type][piece_number].row-row) <= 1 && abs(board->pieces[piece_type][piece_number].col-col) <= 1) {
                return 1;
              } else if (col == 2 && board->pieces[piece_type][piece_number].has_moved == FALSE && board->pieces[piece_type][6].has_moved == FALSE) {
                return 1;
              } else if (col == 6 && board->pieces[piece_type][piece_number].has_moved == FALSE && board->pieces[piece_type][7].has_moved == FALSE) {
                return 1;
              }
              break;
    case 'Q' :
    case 'q' :if (board->pieces[piece_type][piece_number].row == row || board->pieces[piece_type][piece_number].col == col ||
                  abs(board->pieces[piece_type][piece_number].row-row) == abs(board->pieces[piece_type][piece_number].col-col)) {
                return 1;
              }
              break;
    case 'R' :
    case 'r' :if (board->pieces[piece_type][piece_number].row == row || board->pieces[piece_type][piece_number].col == col) {
                return 1;
              }
              break;
    case 'N' :
    case 'n' :if (abs(board->pieces[piece_type][piece_number].row - row) == 1 && abs(board->pieces[piece_type][piece_number].col - col) == 2) {
                return 1;
              } else if (abs(board->pieces[piece_type][piece_number].row - row) == 2 && abs(board->pieces[piece_type][piece_number].col - col) == 1) {
                return 1;
              }
              break;
    case 'B' :
    case 'b' :if (abs(board->pieces[piece_type][piece_number].row-row) == abs(board->pieces[piece_type][piece_number].col-col)) {
                return 1;
              }
              break;
    case 'P' :if (board->pieces[piece_type][piece_number].row == 1 && row == 3 && col == board->pieces[piece_type][piece_number].col) {
                return 1;
              } else if (board->pieces[piece_type][piece_number].col == col && board->pieces[piece_type][piece_number].row + 1 == row) {
                return 1;
              } else if ((board->pieces[piece_type][piece_number].col + 1 == col || board->pieces[piece_type][piece_number].col - 1 == col) && board->pieces[piece_type][piece_number].row + 1 == row) {
                return 1;
              }
              break;
    case 'p' :if (board->pieces[piece_type][piece_number].row == 6 && row == 4 && col == board->pieces[piece_type][piece_number].col) {
                return 1;
              } else if (board->pieces[piece_type][piece_number].col == col && board->pieces[piece_type][piece_number].row - 1 == row) {
                return 1;
              } else if ((board->pieces[piece_type][piece_number].col + 1 == col || board->pieces[piece_type][piece_number].col - 1 == col) && board->pieces[piece_type][piece_number].row - 1 == row) {
                return 1;
              }
              break;
  }
  return 0;
}

int check_collision(board_t *board, int piece_type, int piece_number, int row, int col) {

  char type = board->pieces[piece_type][piece_number].name;
  int i, x, y, distance;

  if (board->pieces[piece_type][piece_number].row - row > 0) {
    //we are traveling in the up direction:
    y = -1;
  } else if (board->pieces[piece_type][piece_number].row - row == 0) {
    //There is no y component to the trip:
    y = 0;
  } else {
    //we are traveling in the down direction:
    y = 1;
  }

  if (board->pieces[piece_type][piece_number].col - col > 0) {
    //we are traveling in the left direction:
    x = -1;
  } else if (board->pieces[piece_type][piece_number].col - col == 0) {
    //there is no x component to the trip:
    x = 0;
  } else {
    //we are traveling in the right direction:
    x = 1;
  }

  if (x != 0 && y != 0) {
    distance = abs(board->pieces[piece_type][piece_number].row - row);
  } else if (x == 0) {
    distance = abs(board->pieces[piece_type][piece_number].row - row);
  } else {
    distance = abs(board->pieces[piece_type][piece_number].col - col);
  }

  switch (type) {

    case 'K' :if (distance > 1) {
                if (col == 6 && board->pieces[piece_type][7].has_moved == FALSE) {
                  if (board->board[0][5] == ' ' && board->board[0][6] == ' ') {
                    return 2;
                  }

                } else if (col == 2 && board->pieces[piece_type][6].has_moved == FALSE) {
                    if (board->board[0][1] == ' ' && board->board[0][2] == ' ' && board->board[0][3] == ' ') {
                      return 2;
                    } else {
                      return 0;
                    }
                  }
              }
    case 'Q' :
    case 'R' :
    case 'B' :for (i = 1; i <= distance; i++) {
                if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'A'
                    && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'Z') {
                    //if there is a piece on the path of the same color
                    return 0;
                } else if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'a'
                    && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'z'
                    && board->pieces[piece_type][piece_number].row + i*y == row && board->pieces[piece_type][piece_number].col + i*x == col) {
                    //if it is the last point and it has reached a piece of a different color

                    board->pieces[piece_type][piece_number].has_moved = TRUE;

                    //TODO: take the piece
                    return 3;
                } else if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'a'
                    && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'z'
                    && (board->pieces[piece_type][piece_number].row + i*y != row || board->pieces[piece_type][piece_number].col + i*x != col)) {
                    //if it has not yet reached the last point and it has reached a piece of a different color
                    return 0;
                }
              }
              break;

    case 'k' :if (distance > 1) {
                if (col == 6 && board->pieces[piece_type][7].has_moved == FALSE) {
                  if (board->board[0][5] == ' ' && board->board[0][6] == ' ') {
                    return 2;
                  }
                } else if (col == 2 && board->pieces[piece_type][6].has_moved == FALSE) {
                    if (board->board[0][1] == ' ' && board->board[0][2] == ' ' && board->board[0][3] == ' ') {
                      return 2;
                    } else {
                      return 0;
                    }
                  }
              }
    case 'q' :
    case 'r' :
    case 'b' :for (i = 1; i <= distance; i++) {
                if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'a'
                    && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'z') {
                    //if there is a piece on the path of the same color
                    return 0;
                } else if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'A'
                    && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'Z'
                    && board->pieces[piece_type][piece_number].row + i*y == row && board->pieces[piece_type][piece_number].col + i*x == col) {
                    //if it is the last point and it has reached a piece of a different color

                    board->pieces[piece_type][piece_number].has_moved = TRUE;

                    //TODO: take the piece
                    return 3;
                } else if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'A'
                    && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'Z'
                    && (board->pieces[piece_type][piece_number].row + i*y != row || board->pieces[piece_type][piece_number].col + i*x != col)) {
                    //if it has not yet reached the last point and it has reached a piece of a different color
                    return 0;
                }
              }
              break;

    case 'N' :if (board->board[row][col] > 'A' && board->board[row][col] < 'Z') {
                return 0;
              } else if (board->board[row][col] > 'a' && board->board[row][col] < 'z') {
                //TODO: take the piece
                return 3;
              }
              break;

    case 'n' :if (board->board[row][col] > 'a' && board->board[row][col] < 'z') {
                return 0;
              } else if (board->board[row][col] > 'A' && board->board[row][col] < 'Z') {
                //TODO: take the piece
                return 3;
              }
              break;

    case 'P' :if (x != 0) {
                if (board->board[row][col] > 'a' && board->board[row][col] < 'z') {
                  //TODO: take the piece
                  return 3;
                } else if (board->pieces[piece_type][piece_number].row == 4 && (board->board[board->pieces[piece_type][piece_number].row][board->pieces[piece_type][piece_number].col+1] == 'p'
                          || board->board[board->pieces[piece_type][piece_number].row][board->pieces[piece_type][piece_number].col-1] == 'p')
                          && board->board[row][col] == ' ') {
                  //en passant:
                  //TODO: take the piece
                  return 4;
                  }
                else {
                  return 0;
                }
              } else {
                  for (i = 1; i <= distance; i++) {
                    if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'A'
                        && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'Z') {
                        //if there is a piece on the path of the same color
                        return 0;
                    } else if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'a'
                        && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'z'
                        && (board->pieces[piece_type][piece_number].row + i*y != row || board->pieces[piece_type][piece_number].col + i*x != col)) {
                        //if it has not yet reached the last point and it has reached a piece of a different color
                        return 0;
                    }
                  }
              }
              break;

    case 'p' :if (x != 0) {
                if (board->board[row][col] > 'A' && board->board[row][col] < 'Z') {
                  //TODO: take the piece
                  return 3;
                } else if (board->pieces[piece_type][piece_number].row == 4 && (board->board[board->pieces[piece_type][piece_number].row][board->pieces[piece_type][piece_number].col+1] == 'p'
                          || board->board[board->pieces[piece_type][piece_number].row][board->pieces[piece_type][piece_number].col-1] == 'P')
                          && board->board[row][col] == ' ') {
                  //en passant:
                  //TODO: take the piece
                    return 4;
                  } else {
                    return 0;
                  }
              } else {
                  for (i = 1; i <= distance; i++) {
                    if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'a'
                        && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'z') {
                        //if there is a piece on the path of the same color
                        return 0;
                    } else if (board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] > 'A'
                        && board->board[board->pieces[piece_type][piece_number].row + i*y][board->pieces[piece_type][piece_number].col + i*x] < 'Z'
                        && (board->pieces[piece_type][piece_number].row + i*y != row || board->pieces[piece_type][piece_number].col + i*x != col)) {
                        //if it has not yet reached the last point and it has reached a piece of a different color
                        return 0;
                    }
                  }
              }
              break;
  }

  board->pieces[piece_type][piece_number].has_moved = TRUE;
  return 1;
}


int take_piece(board_t *board, int piece_type, int piece_number, int enPassant) {
  int i = 0;

  int row = board->pieces[piece_type][piece_number].row;
  int col = board->pieces[piece_type][piece_number].col;
  int opp_type = abs(piece_type-1);

  if (!enPassant) {
    for (i = 0; i < 16; i++) {
      if (board->pieces[opp_type][i].row == row && board->pieces[opp_type][i].col == col) {
        board->pieces[opp_type][i].taken = TRUE;
        return i;
      }
    }
  } else {
    for (i = 0; i < 16; i++) {
      if (board->pieces[piece_type][piece_number].name == 'P') {
        if (board->pieces[opp_type][i].row == row - 1 && board->pieces[opp_type][i].col == col) {
          board->pieces[opp_type][i].taken = TRUE;
          return i;
        }
      }
      if (board->pieces[piece_type][piece_number].name == 'p') {
        if (board->pieces[opp_type][i].row == row + 1 && board->pieces[opp_type][i].col == col) {
          board->pieces[opp_type][i].taken = TRUE;
          return i;
        }
      }
    }
  }

  return 0;
}
