#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ai.h"
#include "player.h"
#include "global.h"


void place_pieces(board_t *board){
  int i, row;
  char piece, col;
  for(i = 0; i < 16; i++){
    board->pieces[WHITE][i].taken = TRUE;
    board->pieces[WHITE][i].row = -1;
    board->pieces[WHITE][i].col = -1;
    board->pieces[BLACK][i].taken = TRUE;
    board->pieces[BLACK][i].row = -1;
    board->pieces[BLACK][i].col = -1;
  }
  draw(board, stdout);
  printf("Add pieces, format = <piece> at <cell>\n");
  printf("Type d to finish\n");
  i = 1;
  while(i){
    printf("Add piece: ");
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
      draw(board, stdout);
    }
  }
}

void parse_cmd_options(int argc, char **argv, board_t *board){
  int i;
  char *filename;
  for(i = 1; i < argc; i++){
    if(strcmp(argv[i], "-p") == 0){
      place_pieces(board);
    }else if(strcmp(argv[i], "-d") == 0){
      DEBUG = 1;
      printf("Debugging mode active!\n");
    }else if(strcmp(argv[i], "-o") == 0){
      filename = argv[++i];
      fd = fopen(filename, "w");
      if(fd == NULL){
        printf("Error opening file %s for output\n", filename);
        exit(1);
      }
    }else{
      printf("Error: usage. Proper usage:\n");
      printf("./chess [-p] [-d] [-o <filename>]\n");
      exit(0);
    }
  }
}

int main(int argc, char** argv){
  int play, num_players;
  board_t *board;
  player_t *player1, *player2;

  player1 = (player_t *) malloc(sizeof(player_t));
  player2 = (player_t *) malloc(sizeof(player_t));
  board = create_board();
  draw(board, stdout);
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
  parse_cmd_options(argc, argv, board);
  play = 1;
  printf("Beginning game\n");
  while(play){
    if(is_draw(board)){
      printf("Draw!\n");
      return 0;
    }
    play = move(player1, board);
    if(fd != NULL){
      draw(board, fd);
    }
    draw(board, stdout);
    if(play){
      if(is_draw(board)){
        printf("Draw!\n");
        return 0;
      }
      play = move(player2, board);
      draw(board, stdout);
      if(fd != NULL){
        draw(board, fd);
      }
    }
  }

  return 0;
}
