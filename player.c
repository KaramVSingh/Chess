#include <stdio.h>
#include "player.h"
#include "board.h"

int move(player_t *player){
  int n, src_row, dst_row;
  char src_col, dst_col;

  if(player->is_human){
    printf("Enter 2 coordinates to make a move, or 1 to see potential moves for that piece\n");
    do{
      printf("Enter move: ");
      n = scanf(" %c%d to %c%d", &src_col, &src_row, &dst_col, &dst_row);
      if(n == 2){
        //show potential moves for that piece
      }else if(n == 4){
        //move that piece if possible
      }else{
        printf("Format error. Proper format is: a1 to a2 OR e4\n");
      }
    }while(n != 2 && n != 4);
  }
  return 0;
}
