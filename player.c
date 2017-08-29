#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "ai.h"
#include "move.h"

int validate_move(move_t *move, move_t *moves, int length){
  int i;

  for(i = 0; i < length; i++){
    if(moves[i].src_row == move->src_row &&
       moves[i].src_col == move->src_col &&
       moves[i].dst_row == move->dst_row &&
       moves[i].dst_col == move->dst_col){
         *move = moves[i];
         printf("Valid move!\n");
         return 1;
       }
  }
  printf("Invalid move!\n");
  return 0;
}


int move(player_t *player, board_t *board){
  int src_row, dst_row, color;
  char src_col, dst_col, moved, taken;
  move_t move, player_move;

  if(player->is_human){
    move.children = generate_moves(board, player->piece_type, &move.length);
    do{
      printf("Enter move: ");
      scanf(" %c%d to %c%d", &src_col, &src_row, &dst_col, &dst_row);
      src_col = (src_col - 1) % 8;
      dst_col = (dst_col - 1) % 8;
      src_row = 8 - src_row;
      dst_row = 8 - dst_row;
      moved = board->board[src_row][(int)src_col];
      taken = board->board[dst_row][(int)dst_col];
      color = 1 - moved/96;
      player_move = create_move(moved, taken, src_row, src_col, dst_row, dst_col, NOMOVE, color);
    }while(!validate_move(&player_move, move.children, move.length));
    move_piece(board, player_move, color);
    print_move(player_move);
    return 1;
  }else{
    return make_move(board, player->piece_type);
  }

  return 1;
}
