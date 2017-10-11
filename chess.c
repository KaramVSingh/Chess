#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ai.h"
#include "player.h"

int main(int argc, char** argv){
  int play, num_players, i, row;
  char piece, col;
  board_t *board;
  player_t *player1, *player2;

  player1 = (player_t *) malloc(sizeof(player_t));
  player2 = (player_t *) malloc(sizeof(player_t));
  board = create_board();
  draw(board);
  printf("Enter number of (human) players (0-2): ");
  scanf("%d", &num_players);
  if(num_players == 0){
    player1->is_human = FALSE;
    printf("Enter difficulty for player 1 (0-%d): ", MAX_DIFFICULTY);
    scanf("%d", &(player1->difficulty));
    if(player1->difficulty < 0 || player1->difficulty > MAX_DIFFICULTY){
      printf("ERROR: Invalid difficulty\n");
      return 0;
    }
    player1->piece_type = WHITE;
    player2->is_human = FALSE;
    printf("Enter difficulty for player 2 (0-%d): ", MAX_DIFFICULTY);
    scanf("%d", &(player2->difficulty));
    if(player2->difficulty < 0 || player2->difficulty > MAX_DIFFICULTY){
      printf("ERROR: Invalid difficulty\n");
      return 0;
    }
    player2->piece_type = BLACK;
    init_ai();
  } else if(num_players == 1){
    player1->is_human = TRUE;
    player1->piece_type = WHITE;
    player2->is_human = FALSE;
    printf("Enter difficulty for player 2 (0-%d): ", MAX_DIFFICULTY);
    scanf("%d", &(player2->difficulty));
    if(player2->difficulty < 0 || player2->difficulty > MAX_DIFFICULTY){
      printf("ERROR: Invalid difficulty\n");
      return 0;
    }
    player2->piece_type = BLACK;
    init_ai();
  }else{
    player1->is_human = TRUE;
    player1->piece_type = WHITE;
    player2->is_human = TRUE;
    player2->piece_type = BLACK;
  }
  if(argc == 1){
    // full game run
  }else if(argc == 2){
    if(strcmp(argv[1], "-p") == 0){
      for(i = 0; i < 16; i++){
        board->pieces[WHITE][i].taken = TRUE;
        board->pieces[BLACK][i].taken = TRUE;
      }
      draw(board);
      printf("Add pieces, format = <piece> at <cell>\n");
      printf("Type d to finish\n");
      i = 1;
      while(i){
        printf("Add piece: ");
        //TODO:fix this
        scanf(" %c", &piece);
        if(piece == 'd'){
          i = 0;
        }else{
          scanf(" at %c%d", &col, &row);
          if(col >= 'a'){
            i = place_piece(board, piece, 8-row, col-97);
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
  printf("Beginning game\n");
  while(play){
    play = move(player1, board);
    draw(board);
    if(play){
      play = move(player2, board);
      draw(board);
    }
  }

  return 0;
}
