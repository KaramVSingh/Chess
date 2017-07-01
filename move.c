#include <stdio.h>
#include <stdlib.h>
#include "move.h"

move_t *generate_moves(board_t *board, int color, int *length){
  move_t *moves, *result, test;
  int i, j, row, col, dst_row, dst_col, shift;
  char c;

  moves = (move_t *)malloc(MAX_MOVES*sizeof(move_t));
  for(j = 0; j < MAX_MOVES; j++){
    moves[j] = create_move('A', 'Z', 0, 0, 0, 0, STANDARD, color);
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
          if(row == (color?6:1)){
            shift = (color?0:32);
            if(board->board[dst_row][dst_col] == ' '){
              test = create_move('Q'+shift, ' ', row, col, dst_row, dst_col, PROMOTION, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
                moves[j] = create_move('R'+shift, ' ', row, col, dst_row, dst_col, PROMOTION, color);
                j++;
                moves[j] = create_move('N'+shift, ' ', row, col, dst_row, dst_col, PROMOTION, color);
                j++;
                moves[j] = create_move('B'+shift, ' ', row, col, dst_row, dst_col, PROMOTION, color);
                j++;
              }
              undo_move(board, test, color);
            }
            if(col + 1 < MAX_COLS){
              dst_col = col + 1;
              if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
                test = create_move('Q'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                  moves[j] = create_move('R'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moves[j] = create_move('N'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moves[j] = create_move('B'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                }
                undo_move(board, test, color);
              }
            }
            if(col - 1 >= 0){
              dst_col = col - 1;
              if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
                test = create_move('Q'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                  moves[j] = create_move('R'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moves[j] = create_move('N'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moves[j] = create_move('B'+shift, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                }
                undo_move(board, test, color);
              }
            }
          }else{
            if(board->board[dst_row][dst_col] == ' '){
              test = create_move(c, ' ', row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
            if(col + 1 < MAX_COLS){
              dst_col = col + 1;
              if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
                test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                }
                undo_move(board, test, color);
              }
            }
            if(col - 1 >= 0){
              dst_col = col - 1;
              if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
                test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                }
                undo_move(board, test, color);
              }
            }
          }
          if(row == (color?1:6)){
            dst_row = row + (color?2:-2);
            dst_col = col;
            if(board->board[dst_row][dst_col] == ' '){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, PREENPASSANT, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          if(enpassant->type == PREENPASSANT){
            if(enpassant->dst_row == row && abs(enpassant->dst_col - col) == 1){
              test = create_move(c, board->board[enpassant->dst_row][enpassant->dst_col], row, col, row + (color?1:-1), enpassant->dst_col, ENPASSANT, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          break;
        case 'K':
        case 'k':
          for(dst_row = row - 1; dst_row <= row + 1; dst_row++){
            for(dst_col = col - 1; dst_col <= col + 1; dst_col++){
              if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
                if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
                  test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
                  move_piece(board, test, color);
                  if(!check(board, color)){
                    moves[j] = test;
                    j++;
                  }
                  undo_move(board, test, color);
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
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row + 1;
          dst_col = col - 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row - 1;
          dst_col = col - 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row - 1;
          dst_col = col + 2;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row + 2;
          dst_col = col + 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row + 2;
          dst_col = col - 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row - 2;
          dst_col = col - 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          dst_row = row - 2;
          dst_col = col + 1;
          if(0 <= dst_row && dst_row < MAX_ROWS && 0 <= dst_col && dst_col < MAX_COLS){
            if(board->board[dst_row][dst_col] == ' ' || board->board[dst_row][dst_col]/96 == color?1:0){
              test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          break;
        case 'R':
        case 'r':
          cardinal_moves(board, board->pieces[color][i], moves, &j);
          break;
        case 'Q':
        case 'q':
          cardinal_moves(board, board->pieces[color][i], moves, &j);
          diagonal_moves(board, board->pieces[color][i], moves, &j);
          break;
        case 'B':
        case 'b':
          diagonal_moves(board, board->pieces[color][i], moves, &j);
          break;
        default:
          break;
      }
    }
  }
  i = 0;
  while(moves[i].moved != 'A'){
    i++;
  }
  result = (move_t *) malloc(i*sizeof(move_t));
  for(j = 0; j < i; j++){
    result[j] = moves[j];
  }
  free(moves);
  moves = NULL;
  *length = i;
  return result;
}


/*
  for debugging purposes, prints out a move in a nice format
*/
void print_move(move_t move){
  switch (move.type){
    case NOMOVE:
    case STANDARD:
    case PREENPASSANT:
      break;
    case ENPASSANT:
      printf("En Passant! ");
      break;
    case CASTLE:
      printf("Castle! ");
      break;
    case PROMOTION:
      printf("Promotion! ");
  }
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
  for(i = 0; i < 16; i++){
    if(board->pieces[color][i].row == move.src_row &&
      board->pieces[color][i].col == move.src_col){
        if(move.type == PROMOTION){
          board->pieces[color][i].name = move.moved;
          switch(move.moved){
            case 'Q':
            case 'q':
              board->pieces[color][i].val = 9;
              break;
            case 'R':
            case 'r':
              board->pieces[color][i].val = 5;
              break;
            case 'N':
            case 'n':
            case 'B':
            case 'b':
              board->pieces[color][i].val = 3;
              break;
          }
        }
        board->pieces[color][i].row = move.dst_row;
        board->pieces[color][i].col = move.dst_col;
      }
    }
    /*if(move.type != PREENPASSANT){
      if(enpassant->type == PREENPASSANT){
        enpassant->type = NOMOVE;
      }else{
        enpassant->type = STANDARD;
      }
    }else{
      enpassant = &move;
    }*/
}

/*
  given a board and a move that was performed, undoes that move
*/
/*
NOTE: undoing multiple times will not work with current implementation of
enpassant. Need actual move history for that
*/
void undo_move(board_t *board, move_t move, int color){
  int i;
  board->board[move.src_row][move.src_col] = move.moved;
  board->board[move.dst_row][move.dst_col] = move.taken;
  if(move.taken != ' '){
    for(i = 0; i < 16; i++){
      if(board->pieces[!color][i].row == move.dst_row &&
        board->pieces[!color][i].col == move.dst_col &&
        board->pieces[!color][i].name == move.taken){
          board->pieces[!color][i].taken = FALSE;
        }
    }
  }
  for(i = 0; i < 16; i++){
    if(board->pieces[color][i].row == move.dst_row &&
      board->pieces[color][i].col == move.dst_col){
        if(move.type == PROMOTION){
          board->pieces[color][i].name = (color?'P':'p');
          board->pieces[color][i].val = 1;
        }
        board->pieces[color][i].row = move.src_row;
        board->pieces[color][i].col = move.src_col;
      }
  }
  /*if(enpassant->type == STANDARD){
    enpassant->type = PREENPASSANT;
  }else if(enpassant->type == PREENPASSANT){
    enpassant->type = NOMOVE;
  }*/
}

move_t create_move(char moved, char taken, int src_row, int src_col, int dst_row, int dst_col, m_type type, int color){
  move_t move;

  move.moved = moved;
  move.taken = taken;
  move.src_row = src_row;
  move.src_col = src_col;
  move.dst_row = dst_row;
  move.dst_col = dst_col;
  move.type = type;
  move.color = color;
  move.value = 0.0;
  move.children = NULL;

  return move;
}

void diagonal_moves(board_t *board, piece_t piece, move_t *moves, int *index){
  int row, col, dst_row, dst_col, done,color;
  char c;
  move_t test;

  row = piece.row;
  col = piece.col;
  c = piece.name;
  color = !(c/96);
  //up left
  dst_row = row - 1;
  dst_col = col - 1;
  done = FALSE;
  while(!done){
    if(dst_row < 0 || dst_col < 0){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_row--;
    dst_col--;
  }
  //up right
  dst_row = row - 1;
  dst_col = col + 1;
  done = FALSE;
  while(!done){
    if(dst_row < 0 || dst_col >= MAX_COLS){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_row--;
    dst_col++;
  }
  //down left
  dst_row = row + 1;
  dst_col = col - 1;
  done = FALSE;
  while(!done){
    if(dst_row >= MAX_ROWS || dst_col < 0){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_row++;
    dst_col--;
  }
  //down right
  dst_row = row + 1;
  dst_col = col + 1;
  done = FALSE;
  while(!done){
    if(dst_row >= MAX_ROWS || dst_col >= MAX_COLS){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_row++;
    dst_col++;
  }
}


void cardinal_moves(board_t *board, piece_t piece, move_t *moves, int *index){
  int row, col, dst_row, dst_col, done,color;
  char c;
  move_t test;

  row = piece.row;
  col = piece.col;
  c = piece.name;
  color = !(c/96);
  //left
  dst_row = row;
  dst_col = col - 1;
  done = FALSE;
  while(!done){
    if(dst_col < 0){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_col--;
  }
  //right
  dst_row = row;
  dst_col = col + 1;
  done = FALSE;
  while(!done){
    if(dst_col >= MAX_COLS){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_col++;
  }
  //up
  dst_row = row - 1;
  dst_col = col;
  done = FALSE;
  while(!done){
    if(dst_row < 0){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_row--;
  }
  //down
  dst_row = row + 1;
  dst_col = col;
  done = FALSE;
  while(!done){
    if(dst_row >= MAX_ROWS){
      done = TRUE;
    }else if(board->board[dst_row][dst_col] != ' '){
      done = TRUE;
      if(board->board[dst_row][dst_col]/96 == color?1:0){
        test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      test = create_move(c, board->board[dst_row][dst_col], row, col, dst_row, dst_col, STANDARD, color);
      move_piece(board, test, color);
      if(!check(board, color)){
        moves[*index] = test;
        (*index)++;
      }
      undo_move(board, test, color);
    }
    dst_row++;
  }
}
