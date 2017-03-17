#include <stdio.h>
#include "player.h"

int move(player_t *player, board_t *board){
  int n, src_row, dst_row, invalid_move = 0, i, castle, status;
  char src_col, dst_col;

  if(player->is_human){
    printf("Enter 2 coordinates to make a move, or 1 to see potential moves for that piece\n");
    do{
      printf("Enter move: ");
      n = scanf("  %c%d to %c%d", &src_col, &src_row, &dst_col, &dst_row);
      if(n == 2){
        //show potential moves for that piece
      }else if(n == 4){
        //move that piece if possible

        //it is an invalid move is (a) there is a piece of the same color at that spot (b) that spot is out of reach for the piece
        //(c) they are moving to the same space (d) none of your pieces are on the first coordinate (e) the player moves into a check
        //(f) The coordinates are within the range of the board

        //TODO: check if the king is in check -
        n = check_status(board, player->piece_type);
        printf("Check_status: %d\n", n);

        //Check if their coordinates are equal
        if ((src_col == dst_col) && (src_row == dst_row)) {
          printf("Invalid: you must move your piece\n");
          invalid_move = TRUE;
        }

        //checks if coordinates are withing board
        else if (src_row < 1 || src_row > 8 || dst_row < 1 || dst_row > 8 || src_col > 'H' || src_col < 'A' || dst_col > 'H' || dst_col < 'A') {
          printf("Invalid: must be within the board\n");
          invalid_move = TRUE;
        }

        //check if initial coordinate is your color
        else if ((board->board[8-src_row][src_col-65] < 'a') && player->piece_type == BLACK) {
          printf("Invalid: must select your piece with initial coordinate\n");
          invalid_move = TRUE;
        }
        else if (((board->board[8-src_row][src_col-65] > 'Z') || (board->board[8-src_row][src_col-65]) < 'A') && player->piece_type == WHITE) {
          printf("Invalid: must select your piece with initial coordinate\n");
          invalid_move = TRUE;
        }

        //checks if move points to a piece of the same color
        else if ((board->board[8-dst_row][dst_col-65] > 'a') && player->piece_type == BLACK) {
          printf("Invalid: cannot place piece where a piece of your color stands\n");
          invalid_move = TRUE;
        }
        else if (((board->board[8-dst_row][dst_col-65] > 'A') && ((board->board[8-dst_row][dst_col-65] < 'Z')) && player->piece_type == WHITE)) {
          printf("Invalid: cannot place piece where a piece of your color stands\n");
          invalid_move = TRUE;
        }

        //if the move is legal, invalid move = false
        else {
          invalid_move = FALSE;

          castle = 0;

          //checks which piece is being moved, and if possible, updates its coordinates
          for(i = 0; i < 16; i++) {
            if(player->piece_type == WHITE) {
              if(board->pieces[WHITE][i].row == (8 - src_row) && board->pieces[WHITE][i].col == (src_col-65)) {
                //We need to check if the final position is valid and if there are any pieces in the way

                if(check_movement(board, WHITE, i, 8 - dst_row, dst_col - 65) && (castle = check_collision(board, WHITE, i, 8 - dst_row, dst_col - 65)) > 0) {
                  invalid_move = FALSE;
                  board->pieces[WHITE][i].row = 8 - dst_row;
                  board->pieces[WHITE][i].col = dst_col - 65;

                  if (castle == 2) {
                    if (dst_col == 'C') {
                      board->pieces[WHITE][6].col = 3;
                    }
                    if (dst_col == 'G') {
                      board->pieces[WHITE][7].col = 5;
                    }
                  }

                  if ((status = check_status(board, WHITE)) != 0) {
                    invalid_move = TRUE;

                    if (castle == 1) {
                      board->pieces[WHITE][i].row = 8 - src_row;
                      board->pieces[WHITE][i].col = src_col - 65;
                    } else {
                      if (board->pieces[WHITE][i].col == 2) {
                        board->pieces[WHITE][i].col = src_col - 65;
                        board->pieces[WHITE][6].col = 0;
                      } else if (board->pieces[WHITE][i].col == 6) {
                        board->pieces[WHITE][i].col = src_col - 65;
                        board->pieces[WHITE][7].col = 7;
                      }
                    }

                    printf("Invalid: Cannot move into check!!\n");
                  } else {
                    if (board->pieces[WHITE][i].col == 6 && castle == 2) {
                      board->pieces[WHITE][i].has_moved = TRUE;
                      board->pieces[WHITE][7].has_moved = TRUE;
                    }
                    if (board->pieces[WHITE][i].col == 2 && castle == 2) {
                      board->pieces[WHITE][i].has_moved = TRUE;
                      board->pieces[WHITE][6].has_moved = TRUE;
                    }
                  }

                } else {
                  invalid_move = TRUE;
                  printf("Invalid: That piece cannot move there/there is a piece in the way\n");

                }
              }
            } else {
              if(board->pieces[BLACK][i].row == (8 - src_row) && board->pieces[BLACK][i].col == (src_col-65)) {
                //We need to check if the final position is valid and if there are any pieces in the way

                if(check_movement(board, BLACK, i, 8 - dst_row, dst_col - 65) && (castle = check_collision(board, BLACK, i, 8 - dst_row, dst_col - 65)) > 0) {
                  invalid_move = FALSE;
                  board->pieces[BLACK][i].row = 8 - dst_row;
                  board->pieces[BLACK][i].col = dst_col - 65;

                  if (castle == 2) {
                    if (dst_col == 'C') {
                      board->pieces[BLACK][6].col = 3;
                    }
                    if (dst_col == 'G') {
                      board->pieces[BLACK][7].col = 5;
                    }
                  }

                  if ((status = check_status(board, BLACK)) != 0) {
                    invalid_move = TRUE;

                    if (castle == 1) {
                      board->pieces[BLACK][i].row = 8 - src_row;
                      board->pieces[BLACK][i].col = src_col - 65;
                    } else {
                      if (board->pieces[BLACK][i].col == 2) {
                        board->pieces[BLACK][i].col = src_col - 65;
                        board->pieces[BLACK][6].col = 0;
                      } else if (board->pieces[BLACK][i].col == 6) {
                        board->pieces[BLACK][i].col = src_col - 65;
                        board->pieces[BLACK][7].col = 7;
                      }
                    }

                    printf("Invalid: Cannot move into check!!\n");
                  } else {
                    if (board->pieces[BLACK][i].col == 6 && castle == 2) {
                      board->pieces[BLACK][i].has_moved = TRUE;
                      board->pieces[BLACK][7].has_moved = TRUE;
                    }
                    if (board->pieces[BLACK][i].col == 2 && castle == 2) {
                      board->pieces[BLACK][i].has_moved = TRUE;
                      board->pieces[BLACK][6].has_moved = TRUE;
                    }
                  }

                } else {
                  invalid_move = TRUE;
                  printf("Invalid: That piece cannot move there/there is a piece in the way\n");

                }
              }
            }
          }

          draw(board);
        }

      }else{
        printf("Format error. Proper format is: a1 to a2 OR e4\n");
      }
    }while((n != 2 && n != 4) || invalid_move);
  }
  return 0;
}
