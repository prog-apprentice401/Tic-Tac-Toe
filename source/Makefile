CFLAGS=-Wall -g
all:	TicTacToe.o
	gcc $(CFLAGS) TicTacToe.o -o ../TicTacToe
	
TicTacToe.o: TicTacToe.c
	gcc $(CFLAGS) -c TicTacToe.c
clean:
	rm -f ../TicTacToe *.o
