#if !defined(GLOBAL_H)
#define GLOBAL_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MAX_ROWS 8
#define MAX_COLS 8
#define WHITE 0
#define BLACK 1
#define TRUE 1
#define FALSE 0
#define CHECK 1
#define CHECKMATE -1
#define NO_CHECK 0

#define MAX_MOVES 220

extern int DEBUG;

extern float knight_positions[8][8];
extern float bishop_positions[8][8];
extern float rook_positions[8][8];
extern float queen_positions[8][8];

#endif