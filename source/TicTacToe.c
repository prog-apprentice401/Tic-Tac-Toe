/*
	This is a simple Tic-%Tac-Toe game written for the
	linux terminal in C. To execute it, one can move it
	to the /usr/bin folder, nd then just type the name
	from anywhere in the file system.

	Written By:	Koustubh Srivastava
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0


typedef enum errors {
	PLAYER_ERROR,
	USED_CELL,
	INVALID_INDEX
} errors;

typedef enum colours {
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	NORMAL,

	DEFAULT = YELLOW
} colours;

typedef enum player {
	player1 = 1,
	player2 = 2
} player;

typedef _Bool bool;

void printRules (void);
int clearBuffer (void);
void printBoard (const char *, const int);
void input (const player, char *, const int);
bool checkWin (const player, const char *, const int);
void switchPlayer (player *);

void handleError (const errors);
void handleWin (const player);

void setColour (const colours colourToSet);

int main (void)
{
	player nowPlaying;
	int sideLength;
	int cellsLeft;
	char *board = NULL;
	bool tie = 0;
	char playAgain = 'y';
	system ("clear");
	
	setColour (BLUE);
	printf ("Hello! Welcome to this game of tic-tac-toe\n"
                "Here are the rules:\n");
	setColour (DEFAULT);
	printRules ();
	
	setColour (RED);
	printf ("Press Enter to continue\n");
	clearBuffer ();
	
	do {
		setColour (DEFAULT);
		system ("clear");
	
		while (1) {
			printf ("Enter the length of the side of the board:\n");
			setColour (RED);
			scanf ("%d", &sideLength);
			setColour (DEFAULT);
			clearBuffer ();
		
			if (sideLength <= 0 || sideLength > 26) {
				setColour (RED);
				printf ("Length Should Be Between 0 And 26!\n");
				setColour (DEFAULT);
			} else {
				break;
			}
		}

		board = malloc (sideLength * sideLength);

		system ("clear");
		for (int i = 0; i < sideLength; i++) {
			for (int j = 0; j < sideLength; j++) {
				board[i * sideLength + j] = ' ';
			}
		}

		cellsLeft = sideLength * sideLength;
		nowPlaying = player2;

		do {
			switchPlayer (&nowPlaying);
	
			printBoard (board, sideLength);
			printf ("Enter the Indexes, %s\n", (nowPlaying == player1) ? "Player 1" : "Player 2");
			input (nowPlaying, board, sideLength);
			system ("clear");

			cellsLeft--;
			if (cellsLeft < 1) {
				tie = 1;
				break;
			}
		} while (!checkWin (nowPlaying, board, sideLength));

		system ("clear");
		printBoard (board, sideLength);

		if (!tie) {
			handleWin (nowPlaying);
		} else {
			setColour (CYAN);
			printf ("It's a Tie!\n");
			setColour (DEFAULT);
		}

		free (board);

		printf ("Do You Want To Play Again? (y or n)");
		setColour (RED);
	} while ((playAgain = getchar ()) != 'n' && playAgain != 'N');
	
	setColour (NORMAL);
	return 0;
}

void printRules (void)
{
	setColour (YELLOW);
	printf ("\nThis is a two player game.\n"  \
                "2) Players take turns entering the row and column number\n"
                "   of the cell they want their symbol to go into.\n"
                "3) Player 1 is assigned 'X' and Player 2 is assigned '0'\n"
                "4) The first player to align as many of their own symbols\n"
                "   as the number of rows, in a row, column, or diagonal wins.\n\n\n");
	setColour (YELLOW);
}

int clearBuffer (void)
{
	int i = 0, dump;

	while ((dump = getchar ()) != '\n'  &&  dump != EOF && dump != '\r') {
		i++;
	}

	return i;
}


void printBoard (const char *board, const int sideLength)
{
	setColour (GREEN);
	printf ("   ");
	for (char i = 'A'; i < sideLength + 'A'; i++) {
		printf (" %c  ", i);
	}
	printf ("\n");
	
	for (int i = 0; i < sideLength; i++) {
		printf ("%2d)", i);
		for (int j = 0; j < sideLength; j++) {
			printf (" %c %c", board[i * sideLength + j], (j < sideLength - 1) ? '\x7C' : '\n');
		}
		if (i != sideLength - 1) {  
			printf ("  ");
			for (int j = 0; j < sideLength; j++) {
				printf ("----");
			}
			printf("\n");
		}
	}

	setColour (DEFAULT);
}


void input (const player nowPlaying, char *board, const int sideLength)
{
	char col;
	int row;

	while (1) {
		setColour (RED);
		scanf ("%c%d", &col, &row);
		setColour (DEFAULT);
		clearBuffer ();

		col = toupper(col) - 'A';  //convert to valid index

		if (row >= sideLength  ||  row < 0  ||  col >= sideLength  ||  col < 0){
			handleError (INVALID_INDEX);
			continue;
		}

		if (board[row * sideLength + col] != ' ') {
			printf ("Cell: %c%d\n", col, row);
			handleError (USED_CELL);
			continue;
		}
		
		break;
	}

	switch (nowPlaying) {
		case player1:
			board[row * sideLength + col] = 'X';
			break;

		case player2:
			board[row * sideLength + col] = '0';
			break;

		default:
			handleError (PLAYER_ERROR);
			break;
	}
}


bool checkWin (const player nowPlaying, const char *board,  const int sideLength)
{
	//functions used only by checkWin ();
	bool checkRows (player, const char *, const int);
	bool checkDiags (player, const char *, const int);
	bool checkCols (player, const char *, const int);

	return (checkRows (nowPlaying, board, sideLength)
                ||  checkCols (nowPlaying, board, sideLength)
                ||  checkDiags(nowPlaying, board, sideLength));
}


bool checkRows (const player nowPlaying, const char *board, const int sideLength)
{
	const char charToSearch = (nowPlaying == player1) ? 'X' : '0';

	for (int i = 0; i < sideLength; i++) {
		for (int j = 0; j < sideLength; j++) {
			if (!(board[i * sideLength + j] == charToSearch)) {
				break;
			} else if (j >= sideLength - 1) {
				return true;
			}
		}
	}

	return false;
}


bool checkCols (const player nowPlaying, const char *board, const int sideLength)
{
	char charToSearch;

	switch (nowPlaying) {
		case player1:
			charToSearch = 'X';
			break;
		case player2:
			charToSearch = '0';
			break;
		default:
			handleError (PLAYER_ERROR);
			break;
	}

	for (int j = 0; j < sideLength; j++) {
		for (int i = 0; i < sideLength; i++) {
			if (board[i * sideLength + j] == charToSearch) {
				if (i >= sideLength - 1) {
					return true;
				}
			} else {
				break;
			}
		}
	}

	return false;
}


bool checkDiags (const player nowPlaying, const char *board, const int sideLength)
{
	char charToSearch;

	switch (nowPlaying) {
		case player1:
			charToSearch = 'X';
			break;

		case player2:
			charToSearch = '0';
			break;

		default:
			handleError (PLAYER_ERROR);
			break;
	}

	//check first diagonal
	for (int i = 0, j = 0; i < sideLength  &&  j < sideLength; i++, j++) {
		if (board[i * sideLength + j] == charToSearch) {
			if (i >= sideLength - 1) {
				return true;
			}
		} else {
			break;
		}
	}

	//check second diagonal
	for (int i = sideLength - 1, j = 0; i >= 0  &&  j < sideLength; i--, j++) {
		if (board[i * sideLength + j] == charToSearch) {
			if (i <= 0) {
				return true;
			}
		} else {
			break;
		}
	}

	return false;
}

void switchPlayer (player *nowPlaying)
{
	switch (*nowPlaying) {
		case player1:
			*nowPlaying = player2;
			break;

		case player2:
			*nowPlaying = player1;
			break;

		default:
			handleError(PLAYER_ERROR);
			break;
	}
}

void handleError (const errors errorCode)
{
	setColour (MAGENTA);
	switch (errorCode) {
		case PLAYER_ERROR:
			printf ("FATAL ERROR!!!\n"
					"Error Maintaining Player Record\n\n");
			exit (-1);
			break;

		case USED_CELL:
			printf ("ERROR!!!\n"
					"Cell Already Used!\n\n");
			break;

		case INVALID_INDEX:
			printf ("ERROR!!!\n"
					"Invalid Index\n\n");
			break;
	
		default:
			printf ("UNKNOWN ERROR!!!\n\n");
	}
	setColour (DEFAULT);
}

void handleWin (const player winner)
{
	printf ("Congratularions %s! You Win!\n", (winner == player1) ? "Player 1" : "Player 2");
	printf ("I Hope You Enjoyed The Game!\n");
}

void setColour (const colours colourToSet)
{
	switch (colourToSet)
	{
		case RED:
			printf ("\033[1;31m");
			break;
		case GREEN:
			printf ("\033[1;32m");
			break;
		case YELLOW:
			printf ("\033[1;33m");
			break;
		case BLUE:
			printf ("\033[1;34m");
			break;
		case MAGENTA:
			printf ("\033[1;35m");
			break;
		case CYAN:
			printf ("\033[1;36m");
			break;
		case NORMAL:
			printf ("\033[0;0m");
			break;
		default:
			break;
	}
}
