#define MAX_ROWS 8
#define MAX_COLS 8
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
  piece_t white[16];
  piece_t black[16];  
} board_t;

board_t * create_board();

void draw(board_t *board);

//might not need this function, but could be handy
int check(board_t *board);
