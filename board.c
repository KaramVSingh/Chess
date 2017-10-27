#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "global.h"


int check(board_t *board, int color);

board_t *create_board(){
  board_t *result;
  int i, j, k;
  int values[] = {100, 9, 3, 3, 3, 3, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1};
  int cols[] = {4, 3, 2, 5, 1, 6, 0, 7, 0, 1, 2, 3, 4, 5, 6, 7};
  char names[] = {'K', 'Q', 'B', 'B', 'N', 'N', 'R', 'R', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};

  result = (board_t *)malloc(sizeof(board_t));

  for(i = 0; i < 16; i++){
    result->pieces[BLACK][i].row = i/8;
    result->pieces[BLACK][i].col = cols[i];
    result->pieces[BLACK][i].name = names[i];
    result->pieces[BLACK][i].val = values[i];
    result->pieces[BLACK][i].taken = 0;
    result->pieces[BLACK][i].color = BLACK;
    result->pieces[WHITE][i].row = 7 - i/8;
    result->pieces[WHITE][i].col = cols[i];
    result->pieces[WHITE][i].name = names[i] + 32;
    result->pieces[WHITE][i].val = values[i];
    result->pieces[WHITE][i].taken = 0;
    result->pieces[WHITE][i].color = WHITE;
  }

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
  int i, j, white, black, white_done, black_done;
  char c;

  update_board(board);

  black = white = 0;
  printf(" ___ ___ ___ ___ ___ ___ ___ ___");
  printf("      White   Black\n");
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      c = board->board[i][j];
      printf("| ");
      if(c > 90) {
        printf(ANSI_COLOR_CYAN "%c " ANSI_COLOR_RESET ,c);
      } else {
        printf(ANSI_COLOR_GREEN "%c " ANSI_COLOR_RESET ,c);
      }
      
    }
    printf("| %d", 8-i);
    white_done = black_done = 0;

    while(white_done < 2 && white < 16){
      if(board->pieces[WHITE][white].taken){
        white_done++;
        printf(ANSI_COLOR_CYAN "   %c" ANSI_COLOR_RESET , board->pieces[WHITE][white].name);
      }
      white++;
    }
    for( ; white_done < 2; white_done++){
      printf("    ");
    }
    while(black_done < 2 && black < 16){
      if(board->pieces[BLACK][black].taken){
        black_done++;
        printf(ANSI_COLOR_GREEN "   %c" ANSI_COLOR_RESET, board->pieces[BLACK][black].name);
      }
      black++;
    }
    printf("\n|___|___|___|___|___|___|___|___|\n");

  }
  printf("  A   B   C   D   E   F   G   H \n");
}

int check_status(board_t *board, int color){
  move_t moves;

  moves.children = generate_moves(board, color, &moves.length);
  if(moves.length == 0){
    return CHECKMATE;
  }else if(check(board, color)){
    return CHECK;
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
      }else if(k == 1 && c == ('K' + shift)){
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
      }else if(k == 1 && c == ('K' + shift)){
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
      }else if(k == 1 && c == ('K' + shift)){
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
      }else if(k == 1 && c == ('K' + shift)){
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
      }else if(k == 1 && c == ('K' + shift)){
        return TRUE;
      }else if(k == 1 && !color && c == 'P'){
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
      }else if(k == 1 && c == ('K' + shift)){
        return TRUE;
      }else if(k == 1 && color && c == 'p'){
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
      }else if(k == 1 && c == ('K' + shift)){
        return TRUE;
      }else if(k == 1 && color && c == 'p'){
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
      }else if(k == 1 && c == ('K' + shift)){
        return TRUE;
      }else if(k == 1 && !color && c == 'P'){
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
  int i = 0, color = WHITE;

  switch (name) {
    case 'K':
      color = BLACK;
    case 'k':
      i = 0;
      break;
    //TODO:should check if piece already taken for these, can use a pawn if need be
    case 'Q':
      color = BLACK;
    case 'q':
      i = 1;
      break;
    case 'B':
      color = BLACK;
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
      color = BLACK;
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
      color = BLACK;
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
      color = BLACK;
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
