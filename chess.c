#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

int main(int argc, char** argv){
  int play, num_players, i, row;
  char piece, col;
  board_t *board;
  player_t *player1, *player2;

  player1 = (player_t *) malloc(sizeof(player_t));
  player2 = (player_t *) malloc(sizeof(player_t));
  board = create_board();
  if(argc == 1){
    draw(board);
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
  }else if(argc == 2){
    if(strcmp(argv[1], "-p") == 0){
      for(i = 0; i < 16; i++){
        board->white[i].taken = TRUE;
        board->black[i].taken = TRUE;
      }
      draw(board);
      printf("Add pieces, format = <piece> at <cell>\n");
      i = 1;
      while(i){
        printf("Add piece: ");
        scanf(" %c at %c%d", &piece, &col, &row);
        if(piece == 'f'){
          i = 0;
        }else{
          if(col > 96){
            i = place_piece(board, piece, 8-row, col-96);
          }else{
            i = place_piece(board, piece, 8-row, col-65);
          }
          if(!i){
            i = 1;
            printf("Error placing piece\n");
          }
          draw(board);
        }
      }
    }else{
      printf("Error: usage\n");
      return 0;
    }

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
