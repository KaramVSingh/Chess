//board h file

typedef struct{
  int player;
  int difficulty;
} board_t;

board_t * create_board();

void draw(board_t *board);
