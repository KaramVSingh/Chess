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

/*
  right now this generates the array of available moves (mostly not return_val
  moves yet) and then randomly selects one
*/
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

/*
  generates an array of all available moves for a given color with the
  board currenlty as it is
*/
move_t *generate_moves(board_t *board, int color){
  move_t *moves;
  int i, j, row, col, dst_row, dst_col;
  char c;

  moves = (move_t *)malloc(MAX_MOVES*sizeof(move_t));
  for(j = 0; j < MAX_MOVES; j++){
    moves[j] = create_move('A', 'Z', 0, 0, 0, 0, color);
  }
  j = 0;
  for(i = 0; i < 16; i++){
    if(!board->pieces[color][i].taken){
      row = board->pieces[color][i].row;
      col = board->pieces[color][i].col;
      c = board->pieces[color][i].name;
      switch(c){
        case 'P':
        case 'p':
          dst_row = row + (color?1:-1);
          dst_col = col;
          if(board->board[dst_row][dst_col] == ' '){
            moves[j] = create_move(c, ' ', row, col, dst_row, dst_col, color);
            j++;
          }
          dst_col = col++;
          if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
            moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
            j++;
          }
          dst_col = col--;
          if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
            moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
            j++;
          }
          break;
        case 'K':
        case 'k':
          for(dst_row = row - 1; dst_row <= row + 1; dst_row++){
            for(dst_col = col - 1; dst_col <= col + 1; dst_col++){
              if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
                if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
                  moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
                  j++;
                }
              }
            }
          }
          break;
        case 'N':
        case 'n':
          dst_row = row + 1;
          dst_col = col + 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row + 1;
          dst_col = col - 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row - 1;
          dst_col = col - 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row - 1;
          dst_col = col + 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row + 2;
          dst_col = col + 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row + 2;
          dst_col = col - 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row - 2;
          dst_col = col - 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          dst_row = row - 2;
          dst_col = col + 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              moves[j] = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, color);
              j++;
            }
          }
          break;
        default:
          moves[j] = create_move('S', 'T', 0, 0, 0, 0, color);
          j++;
          break;
      }
    }
  }
  for(i = 0; i < MAX_MOVES; i++){
    print_move(moves[i]);
  }
  return moves;
}

/*
  for debugging purposes, prints out a move in a nice format
*/
void print_move(move_t move){
  printf("Piece %c moves from %c%d to %c%d", move.moved, (char)(move.src_col + 65), 8-move.src_row,
    (char)(move.dst_col + 65), 8-move.dst_row);
  if(move.taken != ' '){
    printf(" taking piece %c\n", move.taken);
  }else{
    printf("\n");
  }
}

/*
  given a move, modifies the board to reflect the state of the move
  right now, it assumes that the move is valid, can alter that if need
  be later
*/
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

move_t create_move(char moved, char taken, int src_row, int src_col, int dst_row, int dst_col, int color){
  move_t move;

  move.moved = moved;
  move.taken = taken;
  move.src_row = src_row;
  move.src_col = src_col;
  move.dst_row = dst_row;
  move.dst_col = dst_col;
  move.color = color;
  move.value = 0.0;
  move.children = NULL;

  return move;
}
