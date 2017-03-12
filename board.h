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
} piece_t;

typedef struct{
  char board[8][8];
  //made this a type in case there are other properties we want to add
  piece_t pieces[2][16];
} board_t;

board_t * create_board();

void draw(board_t *board);

//might not need this function, but could be handy
int check_status(board_t *board, int color);

int place_piece(board_t *board, char name, int row, int col);
