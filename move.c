#include <stdio.h>
#include <stdlib.h>
#include "move.h"

piece_t *find_piece(board_t *board, int row, int col){
  int i, color;
  char c;

  c = board->board[row][col];
  color = (c > 96)? 0: 1;
  for(i = 0; i < 16; i++){
    if(board->pieces[color][i].row == row &&
      board->pieces[color][i].col == col &&
      board->pieces[color][i].name == c &&
      !board->pieces[color][i].taken){
      return &board->pieces[color][i];
    }
  }
  return NULL;
}

int has_moved(board_t* board, char piece, int start_row, int start_col) {
  move_history_t* temp = board->head;

  if(board->board[start_row][start_col] != piece){
    return 1;
  }

  while(temp != NULL) {
    if(temp->move->moved->name == piece) {
      if(temp->move->src_row == start_row && temp->move->src_col == start_col) {
        return 1;
      }
    }

    temp = temp->next_move;
  }

  return 0;
}

move_t *generate_moves(board_t *board, int color, int *length){
  move_t *moves, *result, test;
  int i, j, row, col, dst_row, dst_col, shift, index;
  char c;
  piece_t *moved, *captured;

  moves = (move_t *)malloc(MAX_MOVES*sizeof(move_t));
  for(j = 0; j < MAX_MOVES; j++){
    moves[j] = create_move(NULL, NULL, 0, 0, 0, 0, NOMOVE, color);
  }
  j = 0;
  for(i = 0; i < 16; i++){
    moved = &board->pieces[color][i];
    if(!moved->taken){
      row = moved->row;
      col = moved->col;
      c = moved->name;
      switch(c){
        case 'P':
        case 'p':
          dst_row = row + (color?1:-1);
          dst_col = col;
          //promotion case
          if(row == (color?6:1)){
            shift = (color?0:32);
            if(board->board[dst_row][dst_col] == ' '){
              captured = find_piece(board, dst_row, dst_col);
              moved->name = 'Q' + shift;
              test = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
                moved->name = 'N' + shift;
                moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                j++;
                moved->name = 'B' + shift;
                moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                j++;
                moved->name = 'R' + shift;
                moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                j++;
              }
              undo_move(board, test, color);
            }
            //checking for capture and promotion
            if(col + 1 < MAX_COLS){
              dst_col = col + 1;
              if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
                captured = find_piece(board, dst_row, dst_col);
                moved->name = 'Q' + shift;
                test = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                  moved->name = 'N' + shift;
                  moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moved->name = 'B' + shift;
                  moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moved->name = 'R' + shift;
                  moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                }
                undo_move(board, test, color);
              }
            }
            if(col - 1 >= 0){
              dst_col = col - 1;
              if(board->board[dst_row][dst_col] != ' ' && board->board[dst_row][dst_col]/96 == color?1:0){
                captured = find_piece(board, dst_row, dst_col);
                moved->name = 'Q' + shift;
                test = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                  moved->name = 'N' + shift;
                  moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moved->name = 'B' + shift;
                  moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                  moved->name = 'R' + shift;
                  moves[j] = create_move(moved, captured, row, col, dst_row, dst_col, PROMOTION, color);
                  j++;
                }
                undo_move(board, test, color);
              }
            }
          }else{
            //standard move
            if(board->board[dst_row][dst_col] == ' '){
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
                captured = find_piece(board, dst_row, dst_col);
                test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
                captured = find_piece(board, dst_row, dst_col);
                test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
                move_piece(board, test, color);
                if(!check(board, color)){
                  moves[j] = test;
                  j++;
                }
                undo_move(board, test, color);
              }
            }
          }
          //initial move two squares
          if(row == (color?1:6)){
            dst_row = row + (color?2:-2);
            dst_col = col;
            //TODO: check if space in front is empty
            if(board->board[dst_row][dst_col] == ' '){
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, PREENPASSANT, color);
              move_piece(board, test, color);
              if(!check(board, color)){
                moves[j] = test;
                j++;
              }
              undo_move(board, test, color);
            }
          }
          //enpassant
          if(enpassant.type == PREENPASSANT){
            if(enpassant.dst_row == row && abs(enpassant.dst_col - col) == 1){
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, row + (color?1:-1), enpassant.dst_col, ENPASSANT, color);
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
                  captured = find_piece(board, dst_row, dst_col);
                  test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
          int start_row = (c == 'K' ? 0: 7);
          char r = (c == 'K' ? 'R': 'r');
          int can_castle = 1;
          if(!has_moved(board, c, start_row, 4)) {

            if(!has_moved(board, r, start_row, 0)){
              for(index = 1; index < 4; index++) {
                if(board->board[start_row][index] != ' '){
                  can_castle = 0;
                }
              }
              if(can_castle){
                for(index = 3; index > 0; index--){
                  test = create_move(moved, NULL, row, col, start_row, index, STANDARD, color);
                  move_piece(board, test, color);
                  if(check(board, color)){
                    can_castle = 0;
                  }
                  undo_move(board, test, color);
                }
              }
              if(can_castle){
                test = create_move(moved, NULL, row, col, start_row, 1, CASTLE, color);
                moves[j] = test;
                j++;
              }
            }
            can_castle = 1;
            if(!has_moved(board, r, start_row, 7)){
              for(index = 5; index < 7; index++) {
                if(board->board[start_row][index] != ' '){
                  can_castle = 0;
                }
              }
              if(can_castle){
                for(index = 6; index > 4; index--){
                  test = create_move(moved, NULL, row, col, start_row, index, STANDARD, color);
                  move_piece(board, test, color);
                  if(check(board, color)){
                    can_castle = 0;
                  }
                  undo_move(board, test, color);
                }
              }
              if(can_castle){
                test = create_move(moved, NULL, row, col, start_row, 6, CASTLE, color);
                moves[j] = test;
                j++;
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
              captured = find_piece(board, dst_row, dst_col);
              test = create_move(moved, captured, row, col, dst_row, dst_col, STANDARD, color);
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
          cardinal_moves(board, moved, moves, &j);
          break;
        case 'Q':
        case 'q':
          cardinal_moves(board, moved, moves, &j);
          diagonal_moves(board, moved, moves, &j);
          break;
        case 'B':
        case 'b':
          diagonal_moves(board, moved, moves, &j);
          break;
        default:
          break;
      }
    }
  }
  i = 0;
  while(moves[i].moved != NULL){
    i++;
  }
  result = (move_t *) malloc(i*sizeof(move_t));
  for(j = 0; j < i; j++){
    result[j] = moves[j];
  }
  free(moves);
  moves = NULL;
  *length = i;

  //print_history(board);

  return result;
}


/*
  for debugging purposes, prints out a move in a nice format
*/
void print_move(move_t move){
  switch (move.type){
    case NOMOVE:
    case STANDARD:
      break;
    case PREENPASSANT:
      printf("Pre En Passant! ");
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
  if(move.type == CASTLE){
    if(move.dst_col < 4){
      printf("King swaps with the left Rook\n");
    }else{
      printf("King swaps with the right Rook\n");
    }
  }else{
    printf("Piece %c moves from %c%d to %c%d", move.moved->name, (char)(move.src_col + 65), 8-move.src_row,
      (char)(move.dst_col + 65), 8-move.dst_row);
    if(move.captured != NULL){
      printf(" taking piece %c\n", move.captured->name);
    }else{
      printf("\n");
    }
  }
}

/*
  given a move, modifies the board to reflect the state of the move
  right now, it assumes that the move is valid, can alter that if need
  be later
*/
void move_piece(board_t *board, move_t move, int color){
  add_node(board, move);

  board->board[move.src_row][move.src_col] = ' ';
  board->board[move.dst_row][move.dst_col] = move.moved->name;
  if(move.type == CASTLE){
    if(move.dst_col == 6){
      board->board[move.src_row][7] = ' ';
      board->board[move.src_row][5] = move.moved->color? 'R':'r';
      board->pieces[color][7].row = move.src_row;
      board->pieces[color][7].col = 5;
      board->pieces[color][0].row =move.src_row;
      board->pieces[color][0].col = 6;
    }else{
      board->board[move.src_row][0] = ' ';
      board->board[move.src_row][2] = move.moved->color? 'R':'r'; //TODO: check this
      board->pieces[color][6].row = move.src_row;
      board->pieces[color][6].col = 2;
      board->pieces[color][0].row =move.src_row;
      board->pieces[color][0].col = 1;
    }
    return;
  }
  if(move.captured != NULL){
    move.captured->taken = TRUE;
  }

  if(move.type == PROMOTION){
    switch(move.moved->name){
      case 'Q':
      case 'q':
        move.moved->val = 9;
        break;
      case 'R':
      case 'r':
        move.moved->val = 5;
        break;
      case 'N':
      case 'n':
      case 'B':
      case 'b':
        move.moved->val = 3;
        break;
    }
  }
  move.moved->row = move.dst_row;
  move.moved->col = move.dst_col;
    if(move.type != PREENPASSANT){
      if(enpassant.type == PREENPASSANT){
        enpassant.type = STANDARD;
      }else{
        enpassant.type = NOMOVE;
      }
    }else{
      enpassant.moved = move.moved;
      enpassant.captured = move.captured;
      enpassant.src_row = move.src_row;
      enpassant.src_col = move.src_col;
      enpassant.dst_row = move.dst_row;
      enpassant.dst_col = move.dst_col;
      enpassant.type = move.type;
      enpassant.color = move.color;
    }
}

/*
  given a board and a move that was performed, undoes that move
*/
/*
NOTE: undoing multiple times will not work with current implementation of
enpassant. Need actual move history for that
*/
void undo_move(board_t *board, move_t move, int color){
  remove_node(board);
  board->board[move.src_row][move.src_col] = move.moved->name;
  if(move.captured != NULL){
    board->board[move.dst_row][move.dst_col] = move.captured->name;
  }else{
    board->board[move.dst_row][move.dst_col] = ' ';
  }
  
  if(move.captured != NULL){
    move.captured->taken = FALSE;
  }
  if(move.type == PROMOTION){
    move.moved->name = (color?'P':'p');
    move.moved->val = 1;
  }else if(move.type == CASTLE){
    if(move.dst_col == 6){
      board->pieces[color][7].col = 0;
    }else{
      board->pieces[color][6].col = 0;
    }
  }
  move.moved->row = move.src_row;
  move.moved->col = move.src_col;
  if(enpassant.type == STANDARD){
    enpassant.type = PREENPASSANT;
  }else if(enpassant.type == PREENPASSANT){
    enpassant.type = NOMOVE;
  }
}

move_t create_move(piece_t *moved, piece_t *captured, int src_row, int src_col, int dst_row, int dst_col, m_type type, int color){
  move_t move;

  move.moved = moved;
  move.captured = captured;
  move.src_row = src_row;
  move.src_col = src_col;
  move.dst_row = dst_row;
  move.dst_col = dst_col;
  move.type = type;
  move.color = color;
  move.value = 0.0;
  move.children = NULL;
  move.length = 0;

  return move;
}

void diagonal_moves(board_t *board, piece_t *piece, move_t *moves, int *index){
  int row, col, dst_row, dst_col, done,color;
  move_t test;
  piece_t *captured;

  row = piece->row;
  col = piece->col;
  color = piece->color;
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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


void cardinal_moves(board_t *board, piece_t *piece, move_t *moves, int *index){
  int row, col, dst_row, dst_col, done,color;
  move_t test;
  piece_t *captured;

  row = piece->row;
  col = piece->col;
  color = piece->color;
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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
        captured = find_piece(board, dst_row, dst_col);
        test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
        move_piece(board, test, color);
        if(!check(board, color)){
          moves[*index] = test;
          (*index)++;
        }
        undo_move(board, test, color);
      }
    }else{
      captured = find_piece(board, dst_row, dst_col);
      test = create_move(piece, captured, row, col, dst_row, dst_col, STANDARD, color);
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

void print_history(board_t* board) {
  if(!board->head) {
    printf("NULL\n");
    return;
  }

  move_history_t* current_move;
  current_move = board->head;
  while(current_move != NULL) {
    printf("move #%d\n", current_move->index);
    printf("\t(%c%d) -> (%c%d)\n", current_move->move->src_col + 65, 8 - current_move->move->src_row, current_move->move->dst_col + 65, 8 - current_move->move->dst_row);
    current_move = current_move->next_move;
  }
}

void add_node(board_t* board, move_t move) {
  move_history_t* new_head_pointer;
  new_head_pointer = (move_history_t*)malloc(1*sizeof(move_history_t));
  new_head_pointer->move = (move_t*)malloc(1*sizeof(move_t));
  if(board->head == NULL) {
    new_head_pointer->index = 0;
  } else {
    new_head_pointer->index = board->head->index + 1;
  }


  new_head_pointer->next_move = board->head;

  new_head_pointer->move->moved = move.moved;
  new_head_pointer->move->captured = move.captured;
  new_head_pointer->move->src_row = move.src_row;
  new_head_pointer->move->src_col = move.src_col;
  new_head_pointer->move->dst_row = move.dst_row;
  new_head_pointer->move->dst_col = move.dst_col;
  new_head_pointer->move->type = move.type;
  new_head_pointer->move->color = move.color;
  new_head_pointer->move->value = move.value;
  new_head_pointer->move->children = move.children;
  new_head_pointer->move->length = move.length;

  board->head = new_head_pointer;
}

void remove_node(board_t* board) {
  move_history_t* store = board->head;
  if(board->head) {
    board->head = board->head->next_move;
    free(store->move);
  }

  free(store);
}

move_t* get_node(board_t* board, int index) {
  //currently it indexes with the first move being move 0, but this might need to be flipped
  //currently also returning the head if there is an issue
  move_history_t* current_move;
  current_move = board->head;

  while(current_move != NULL) {
    if(current_move->index == index) {
      return current_move->move;
    }
  }

  return board->head->move;
}

int contains_node(board_t* board, move_t move) {
  move_history_t* current_move;
  current_move = board->head;

  while(current_move != NULL) {
    if(move_equals(*(current_move->move), move)) {
      return 1;
    }
  }

  return 0;
}

int move_equals(move_t move1, move_t move2) {
  //checks all parameters but this might want to be changed
  

  return 0;
}
