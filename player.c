#include <stdio.h>
#include <stdlib.h>
#include "player.h"

int move(player_t *player, piece_t *board){
  int n, src_row, dst_row, invalid_move = 0, i, instruction, status, hold, opp_type;
  int src_col, dst_col;

  if(player->is_human){
    printf("Enter 2 coordinates to make a move, or 1 to see potential moves for that piece\n");
    do {
      printf("Enter move: ");
      n = scanf("  %c%d to %c%d", &src_col, &src_row, &dst_col, &dst_row);
      opp_type = abs(player->piece_type - 1);
      if(n == 2){
        //show potential moves for that piece
      }else if(n == 4){
        //move that piece if possible

        //it is an invalid move is (a) there is a piece of the same color at that spot (b) that spot is out of reach for the piece
        //(c) they are moving to the same space (d) none of your pieces are on the first coordinate (e) the player moves into a check
        //(f) The coordinates are within the range of the board

        //TODO: check if the king is in check -
        if (check_status(board, player->piece_type) == CHECKMATE) {
          if (player->piece_type) {
            printf("Checkmate! %s team wins!!\n", "WHITE");
          } else {
            printf("Checkmate! %s team wins!!\n", "BLACK");
          }
          return 0;
        }

        //Check if their coordinates are equal
        if ((src_col == dst_col) && (src_row == dst_row)) {
          printf("Invalid: you must move your piece\n");
          invalid_move = TRUE;
        }

        //checks if coordinates are withing board
        src_col = src_col%32 - 1;
        dst_col = dst_col%32 - 1;
        src_row -= 1;
        dst_row -= 1;
        else if (src_row < 0 || src_row > 7 || dst_row < 0 || dst_row > 7 || src_col > 7 || src_col < 0 || dst_col > 7 || dst_col < 0) {
          printf("Invalid: must be within the board\n");
          invalid_move = TRUE;
        }

        //check if initial coordinate is your color
        else if ((board->board[src_row][src_col].color == player->piece_type) {
          printf("Invalid: must select your piece with initial coordinate\n");
          invalid_move = TRUE;
        }

        //checks if move points to a piece of the same color
        else if (board->board[dst_row][dst_col].color == player->piece_type) {
          printf("Invalid: cannot place piece where a piece of your color stands\n");
          invalid_move = TRUE;
        }

        //if the move is legal, invalid move = false
        else {
          invalid_move = FALSE;

          instruction = 0;

          //checks which piece is being moved, and if possible, updates its coordinates
          for(i = 0; i < 16; i++) {
              if(board->pieces[player->piece_type][i].taken == FALSE) {
                //We need to check if the final position is valid and if there are any pieces in the way

                if(check_movement(board, player->piece_type, i, 8 - dst_row, dst_col - 65) && (instruction = check_collision(board, player->piece_type, i, 8 - dst_row, dst_col - 65)) > 0) {
                  invalid_move = FALSE;
                  board->pieces[player->piece_type][i].row = 8 - dst_row;
                  board->pieces[player->piece_type][i].col = dst_col - 65;

                  if (instruction == 2) {
                    if (dst_col == 'C') {
                      board->pieces[player->piece_type][6].col = 3;
                    }
                    if (dst_col == 'G') {
                      board->pieces[player->piece_type][7].col = 5;
                    }
                  }

                  if (instruction == 3) {
                    hold = take_piece(board, player->piece_type, i, FALSE);
                  }

                  if (instruction == 4) {
                    hold = take_piece(board, player->piece_type, i, TRUE);
                  }

                  if ((status = check_status(board, player->piece_type)) != NO_CHECK) {
                    invalid_move = TRUE;

                    if (instruction == 1) {
                      board->pieces[player->piece_type][i].row = 8 - src_row;
                      board->pieces[player->piece_type][i].col = src_col - 65;
                    } else if (instruction == 2) {
                      if (board->pieces[player->piece_type][i].col == 2) {
                        board->pieces[player->piece_type][i].col = src_col - 65;
                        board->pieces[player->piece_type][6].col = 0;
                      } else if (board->pieces[player->piece_type][i].col == 6) {
                        board->pieces[player->piece_type][i].col = src_col - 65;
                        board->pieces[player->piece_type][7].col = 7;
                      }
                    } else if (instruction == 3 || instruction == 4) {
                      board->pieces[player->piece_type][i].col = src_col - 65;
                      board->pieces[player->piece_type][i].row = 8 - src_row;
                      board->pieces[opp_type][hold].taken = FALSE;
                    }
                    printf("Invalid: Cannot move into check!!\n");
                  } else {
                    if (board->pieces[player->piece_type][i].col == 6 && instruction == 2) {
                      board->pieces[player->piece_type][i].has_moved = TRUE;
                      board->pieces[player->piece_type][7].has_moved = TRUE;
                    }
                    if (board->pieces[player->piece_type][i].col == 2 && instruction == 2) {
                      board->pieces[player->piece_type][i].has_moved = TRUE;
                      board->pieces[player->piece_type][6].has_moved = TRUE;
                    }
                  }

                } else {
                  invalid_move = TRUE;
                  printf("Invalid: That piece cannot move there/there is a piece in the way\n");

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

  return 1;
}
