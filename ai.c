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
  move_piece(board, move, color);
  print_move(move);
  return 0;
}

move_t *generate_moves(board_t *board, int color){
  move_t *moves;
  int i, j, row, col, dst_row, dst_col;
  char c;

  moves = (move_t *)malloc(MAX_MOVES*sizeof(move_t));
  for(j = 0; j < MAX_MOVES; j++){
    moves[j].moved = 'A';
    moves[j].taken = 'Z';
    moves[j].src_row = 0;
    moves[j].src_col = 0;
    moves[j].dst_row = 0;
    moves[j].dst_col = 0;
    moves[j].color = color;
    moves[j].value = 0.0;
    moves[j].children = NULL;
  }
  j = 0;
  for(i = 0; i < 16; i++){
    row = board->pieces[color][i].row;
    col = board->pieces[color][i].col;
    c = board->pieces[color][i].name;
    switch(c){
      case 'P':
      case 'p':
        dst_row = row + (color?1:-1);
        dst_col = col;
        if(board->board[dst_row][dst_col] == ' '){
          moves[j].moved = c;
          moves[j].taken = ' ';
          moves[j].src_row = row;
          moves[j].src_col = col;
          moves[j].dst_row = dst_row;
          moves[j].dst_col = dst_col;
          moves[j].color = color;
          moves[j].value = 0.0;
          moves[j].children = NULL;
          j++;
        }
        dst_col = col++;
        if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
          moves[j].moved = c;
          moves[j].taken = board->board[dst_row][dst_col];
          moves[j].src_row = row;
          moves[j].src_col = col;
          moves[j].dst_row = dst_row;
          moves[j].dst_col = dst_col;
          moves[j].color = color;
          moves[j].value = 0.0;
          moves[j].children = NULL;
          j++;
        }
        dst_col = col--;
        if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
          moves[j].moved = c;
          moves[j].taken = board->board[dst_row][dst_col];
          moves[j].src_row = row;
          moves[j].src_col = col;
          moves[j].dst_row = dst_row;
          moves[j].dst_col = dst_col;
          moves[j].color = color;
          moves[j].value = 0.0;
          moves[j].children = NULL;
          j++;
        }
        break;
      default:
        moves[j].moved = 'M';
        moves[j].taken = 'N';
        moves[j].src_row = 0;
        moves[j].src_col = 0;
        moves[j].dst_row = 0;
        moves[j].dst_col = 0;
        moves[j].color = color;
        moves[j].value = 0.0;
        moves[j].children = NULL;
        j++;
        break;
    }
  }
  for(i = 0; i < MAX_MOVES; i++){
    print_move(moves[i]);
  }
  return moves;
}

void print_move(move_t move){
  printf("Piece %c moves from %c%d to %c%d", move.moved, (char)(move.src_col + 65), 8-move.src_row,
    (char)(move.dst_col + 65), 8-move.dst_row);
  if(move.taken != ' '){
    printf(" taking piece %c\n", move.taken);
  }else{
    printf("\n");
  }
}

void move_piece(board_t *board, move_t move, int color){
  int i;
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
}
