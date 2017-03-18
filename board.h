#define MAX_ROWS 8
#define MAX_COLS 8
#define WHITE 1
#define BLACK 0
#define TRUE 1
#define FALSE 0


typedef struct{
  char name;
  int row;
  int col;
  int val;
  int taken;
  int has_moved;
} piece_t;

typedef struct{
  char board[8][8];
  //made this a type in case there are other properties we want to add
  piece_t pieces[2][16];
} board_t;

board_t * create_board();

void update_board(board_t *board);
void draw(board_t *board);

//might not need this function, but could be handy
int check_status(board_t *board, int color);

int place_piece(board_t *board, char name, int row, int col);

//this function checks if a piece's final position is in accordance with its abilities
int check_movement(board_t *board, int piece_type, int piece_number, int row, int col);
//this function checks if the piece has any pieces in its way <- this function can be inclded in "check Movement", i just kept it seperate for now
int check_collision(board_t *board, int piece_type, int piece_number, int row, int col);
//this function simply removes a piece and adds it to the taken list
int take_piece(board_t *board, int piece_type, int piece_number);
