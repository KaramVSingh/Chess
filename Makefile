CC = gcc
CFLAGS = -g -Wall -O0 -Werror -Wshadow -Wwrite-strings

chess: chess.o player.o board.o ai.o move.o global.o
	$(CC) -o chess chess.o player.o board.o ai.o move.o global.o

chess.o: chess.c player.h board.h ai.h global.h
	$(CC) $(CFLAGS) -c chess.c

player.o: player.c player.h board.h move.h
	$(CC) $(CFLAGS) -c player.c

board.o: board.c board.h move.h global.h
	$(CC) $(CFLAGS) -c board.c

move.o: move.c move.h board.h global.h
		$(CC) $(CFLAGS) -c move.c

ai.o: ai.c ai.h board.h move.h global.h
	$(CC) $(CFLAGS) -c ai.c

global.o: global.c global.h
	$(CC) $(CFLAGS) -c global.c


clean:
		rm -f *.o chess
