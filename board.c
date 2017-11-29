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
void draw(board_t * board, FILE *stream){
  int i, j, white, black, white_done, black_done;
  char c;

  update_board(board);

  black = white = 0;
  fprintf(stream, " ___ ___ ___ ___ ___ ___ ___ ___");
  fprintf(stream, "      White   Black\n");
  for(i = 0; i < MAX_ROWS; i++){
    for(j = 0; j < MAX_COLS; j++){
      c = board->board[i][j];
      fprintf(stream, "| ");
      if(c > 90) {
        fprintf(stream, ANSI_COLOR_CYAN "%c " ANSI_COLOR_RESET ,c);
      } else {
        fprintf(stream, ANSI_COLOR_GREEN "%c " ANSI_COLOR_RESET ,c);
      }
      
    }
    fprintf(stream, "| %d", 8-i);
    white_done = black_done = 0;

    while(white_done < 2 && white < 16){
      if(board->pieces[WHITE][white].taken){
        white_done++;
        fprintf(stream, ANSI_COLOR_CYAN "   %c" ANSI_COLOR_RESET , board->pieces[WHITE][white].name);
      }
      white++;
    }
    for( ; white_done < 2; white_done++){
      fprintf(stream, "    ");
    }
    while(black_done < 2 && black < 16){
      if(board->pieces[BLACK][black].taken){
        black_done++;
        fprintf(stream, ANSI_COLOR_GREEN "   %c" ANSI_COLOR_RESET, board->pieces[BLACK][black].name);
      }
      black++;
    }
    fprintf(stream, "\n|___|___|___|___|___|___|___|___|\n");

  }
  fprintf(stream, "  A   B   C   D   E   F   G   H \n");
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

int is_draw(board_t *board){
  int i;
  int color = -1;
  int white_knights = 0, black_knights = 0;
  piece_t white, black;
  for(i = 0; i < 16; i++){
    white = board->pieces[WHITE][i];
    black = board->pieces[BLACK][i];
    if(!white.taken){
      if(white.name == 'q'){
        return FALSE;
      }else if(white.name == 'r'){
        return FALSE;
      }else if(white.name == 'n'){
        if(++white_knights > 1){
          return FALSE;
        }
      }else if(white.name == 'b'){
        int cell_color = (white.row + white.col) % 2;
        if(color == -1){
          color = cell_color;
        }else if(color != cell_color){
          return FALSE;
        }
      }
    }
    if(!black.taken){
      if(black.name == 'q'){
        return FALSE;
      }else if(black.name == 'r'){
        return FALSE;
      }else if(black.name == 'n'){
        if(++black_knights > 1){
          return FALSE;
        }
      }else if(black.name == 'b'){
        int cell_color = (black.row + black.col) % 2;
        if(color == -1){
          color = cell_color;
        }else if(color != cell_color){
          return FALSE;
        }
      }
    }
  }

  move_history_t *temp = board->head;
  int count = 0;
  while(count < 50 && temp != NULL){
    if(temp->move->captured != NULL){
      return FALSE;
    }else if(temp->move->moved->name == 'p' || temp->move->moved->name == 'P'){
      return FALSE;
    }
    count++;
    temp = temp->next_move;
  }
  if(count < 50){
    return FALSE;
  }
  
  return TRUE;
}