#include <stdio.h>
#include <stdlib.h>
#include "player.h"

int main(int argc, char** argv){
  int play, num_players;
  board_t *board;
  player_t *player1, *player2;

  player1 = (player_t *) malloc(sizeof(player_t));
  player2 = (player_t *) malloc(sizeof(player_t));
  board = create_board();
  draw(board);
  if(argc == 1){
    // full game run
    printf("Enter number of (human) players (0-2): ");
    scanf("%d", &num_players);
    if(num_players == 0){
      player1->is_human = FALSE;
      player1->piece_type = WHITE;
      player2->is_human = FALSE;
      player2->piece_type = BLACK;
    } else if(num_players == 1){
      player1->is_human = TRUE;
      player1->piece_type = WHITE;
      player2->is_human = FALSE;
      player2->piece_type = BLACK;
    }else{
      player1->is_human = TRUE;
      player1->piece_type = WHITE;
      player2->is_human = TRUE;
      player2->piece_type = BLACK;
    }
  }else if(argc == 1){
    // do something
  }else{
    // do something else
  }
  play = 1;
  while(play){
    draw(board);
    play = move(player1, board);
    if(play){
      play = move(player2, board);
    }
  }

  return 0;
}
