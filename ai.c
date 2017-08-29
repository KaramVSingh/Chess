#include "ai.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

float weights[8][8];

/*
Calculates the net material for the color on the board by summing the pieces
not taken
*/
float calculate_material(board_t *board, int color){
  int i;
  float result, sign;

  sign = color?1.0:-1.0;
  result = 0.0;
  for(i = 0; i < 16; i++){
    if(!board->pieces[WHITE][i].taken){
      result -= sign*(board->pieces[WHITE][i].val);
    }
    if(!board->pieces[BLACK][i].taken){
      result += sign*(board->pieces[BLACK][i].val);
    }
  }

  return result;
}

void init_ai(){
  int i, j;
  float x, y, val;

  for(i = 0; i < 8; i++){
    if(i < 4){
      x = (float) i;
    }else{
      x = -1.0*i + 7.0;
    }
    for(j = 0; j < 8; j++){
      if(j < 4){
        y = (float) j;
      }else{
        y = -1.0*j + 7.0;
      }
      val = -1.0 + (x + y)/3.0;
      weights[i][j] = val;
      //printf("%.2f\t", val);
    }
    //printf("\n");
  }
  enpassant.type = NOMOVE;
  castle[WHITE].type = NOMOVE;
  castle[BLACK].type = NOMOVE;
  srand(time(NULL));
}

/*
  right now this generates the array of available moves (mostly not return_val
  moves yet) and then randomly selects one
*/
int make_move(board_t *board, int color){
  int i, j, difficulty;
  float max_material = -INFINITY, temp;
  move_t parent, move;
  difficulty = 1, j = 0;

  parent.children = generate_moves(board, color, &parent.length);
  if(difficulty == 1){
    for(i = 0; i < parent.length; i++){
      move_piece(board, parent.children[i], color);
      temp = calculate_material(board, color);
      if(temp > max_material){
        j = 0;
        parent.children[j] = parent.children[i];
        max_material = temp;
        j++;
      }else if (temp == max_material){
        parent.children[j] = parent.children[i];
        j++;
      }
      undo_move(board, parent.children[i], color);
    }
    parent.length = j;
  }

  printf("Number of moves: %d\n", parent.length);
  for(i = 0; i < parent.length; i++){
    print_move(parent.children[i]);
  }
  if(parent.length == 0){
    printf("CHECKMATE\n");
    return 0;
  }
  i = rand() % parent.length;
  //alphabeta(board, &parent, TRUE, 0, -1000, 1000);
  move = parent.children[i];
  move_piece(board, move, color);
  draw(board);
  print_move(move);
  return 0;
}

/*
  generates an array of all available moves for a given color with the
  board currenlty as it is
  TODO: special moves (pawns, castling), and disallow moves that place ai in check
*/
