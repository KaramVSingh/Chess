#include <stdio.h>
#include "player.h"

int move(player_t *player, board_t *board){
  int n, src_row, dst_row, invalid_move = 0;
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

        //TODO: check if the king is in check -
        check(board, player->piece_type);
        //Check if their coordinates are equal
        if ((src_col == dst_col) && (src_row == dst_row)) {
          printf("Invalid: you must move your piece\n");
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
        }

        //TODO: check if king is in check

      }else{
        printf("Format error. Proper format is: a1 to a2 OR e4\n");
      }
    }while((n != 2 && n != 4) || invalid_move);
  }
  return 0;
}
