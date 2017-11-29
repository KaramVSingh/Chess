#include "ai.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "global.h"

#define OPENING 0
#define MIDGAME 1
#define ENDGAME 2
#define OFFENSE 8

float weights[8][8];

int get_mode(board_t *board, int color){
  int i;
  float self = 0.0f, opponent = 0.0f;
  int return_val = OPENING;
  for(i = 1; i < 16; i++){
    if(!board->pieces[!color][i].taken){
      opponent += board->pieces[!color][i].val;
    }
    if(!board->pieces[color][i].taken){
      self += board->pieces[color][i].val;
    }
  }
  if(self >= opponent){
    return_val |= OFFENSE;
  }
  if(self > 9 && opponent > 9){
    return_val |= MIDGAME;
  }else{
    return_val |= ENDGAME;
  }
  return return_val;
}

int available_moves(board_t *board, int color){
  move_t temp;
  temp.children = generate_moves(board, color, &temp.length);
  free(temp.children);

  return color? temp.length : -temp.length;
}
/*
Calculates the net material for the color on the board by summing the pieces
not taken
*/
float calculate_material(board_t *board){
  int i;
  float result;

  result = 0.0;
  for(i = 0; i < 16; i++){
    if(!board->pieces[WHITE][i].taken){
      result -= (board->pieces[WHITE][i].val);
    }
    if(!board->pieces[BLACK][i].taken){
      result += (board->pieces[BLACK][i].val);
    }
  }

  return result;
}

float evaluate_board(board_t *board, int color){
  float value = 0.0f;
  int mode = get_mode(board, color);

  if(mode & MIDGAME){
    value = calculate_material(board);  
  }else if(mode & ENDGAME){
    if(mode & OFFENSE){
      value += available_moves(board, !color);
    }else{
      value += available_moves(board, color);
    }
  }
  

  return value;
}

float alphabeta(board_t *board, move_t move, int color, int depth, float alpha, float beta){
  int i;
  float v, new;

  if(move.length == 0){
    return color? INFINITY: -INFINITY;
  }

  if(depth == 0){
    return evaluate_board(board, color);
  }

  if(color){
    v = -INFINITY;
    for(i = 0; i < move.length; i++){
      move_piece(board, move.children[i], color);
      move.children[i].children = generate_moves(board, !color, &move.children[i].length);
      new = alphabeta(board, move.children[i], !color, depth - 1, alpha, beta);
      undo_move(board, move.children[i], color);
      if(DEBUG){
        printf("Color: %d, v: %1.2f, new: %1.2f, alpha: %1.2f, move: ", color, v, new, alpha);
        print_move(move.children[i]);
      }
      v = (v > new ? v : new);
      free(move.children[i].children);
      move.children[i].children = NULL;
      move.children[i].value = new;
      alpha = (alpha > v? alpha: v);
      if(beta <= v){
        if(DEBUG){
          printf("Pruning, %f <= %f\n", beta, v);
        }
        break;
      }
    }
    return v;
  }else{
    v = INFINITY;
    for(i = 0; i < move.length; i++){
      move_piece(board, move.children[i], color);
      move.children[i].children = generate_moves(board, !color, &move.children[i].length);
      new = alphabeta(board, move.children[i], !color, depth - 1, alpha, beta);
      undo_move(board, move.children[i], color);
      if(DEBUG){
        printf("Color: %d, v: %1.2f, new: %1.2f, beta: %1.2f, move: ", color, v, new, beta);
        print_move(move.children[i]);
      }
      v = (v < new? v : new);
      free(move.children[i].children);
      move.children[i].children = NULL;
      move.children[i].value = new;
      beta = (beta < v? beta: v);
      if(v <= alpha){
        if(DEBUG){
          printf("Pruning, %f <= %f\n", v, alpha);
        }
        break;
      }
    }
    return v;
  }

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
int make_move(board_t *board, int difficulty, int color){
  int i, j, depth;
  float temp;
  float alpha = -INFINITY, beta = INFINITY;
  move_t parent, move;
  j = 0;

  depth = difficulty;

  parent.children = generate_moves(board, color, &parent.length);
  temp = alphabeta(board, parent, color, depth, alpha, beta);
  j = 0;
  for(i = 0; i < parent.length; i++){
    if(parent.children[i].value == temp){
      parent.children[j] = parent.children[i];
      j++;
    }
  }

  if(DEBUG){
    printf("Number of moves: %d\n", j);
    for(i = 0; i < j; i++){
      print_move(parent.children[i]);
    }
  }

  if(j == 0){
    if(check(board, color)){
      printf("CHECKMATE\n");
    }else{
      printf("STALEMATE\n");
    }
    return 0;
  }
  i = rand() % j;
  move = parent.children[i];
  move_piece(board, move, color);
  print_move(move);
  free(parent.children);
  return 1;
}

/*
  generates an array of all available moves for a given color with the
  board currenlty as it is
  TODO: special moves (pawns, castling), and disallow moves that place ai in check
*/
