#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "player.h"

int main(int argc, char** argv){
  int play;
  board_t *board;
  player_t *player1, *player2;

  player1 = (player_t *) malloc(sizeof(player_t));
  player2 = (player_t *) malloc(sizeof(player_t));
  if(argc == 0){
    // full game run
  }else if(argc == 1){
    // do something
  }else{
    // do something else
  }
  board = create_board();
  move = 1;
  while(play){
    draw(board);
    play = move(player1);
    if(play){
      play = move(player2);
    }
  }

  return 0;
}
