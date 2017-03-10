#define MAX_ROWS 8
#define MAX_COLS 8

typedef struct{
  char board[8][8];
  //made this a type in case there are other properties we want to add
} board_t;

board_t * create_board();

void draw(board_t *board);

//might not need this function, but could be handy
int check(board_t *board);
