/*
    This is a simple Tic-%Tac-Toe game written for the
    *nix terminal in C.

    Written By:	Koustubh Srivastava
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

#define printRules()	printf ("\nThis is a two player game.\n"  \
								"2) Players take turns entering the row and column number\n"  \
								"   of the cell they want their symbol to go into.\n"  \
								"3) Player 1 is assigned 'X' and Player 2 is assigned '0'\n"  \
								"4) The first player to align as many of their own symbols\n"  \
								"   as the number of rows, in a row, column, or diagonal wins.\n\n\n");


typedef enum errors
{
	PLAYER_ERROR,
	USED_CELL,
	INVALID_INDEX
} errors;


typedef enum player
{
	player1 = 1,
	player2 = 2
} player;

typedef _Bool bool;

/*function definitions for main*/

int clearBuffer (void);	 //utility function to clear the buffer when needed
void printBoard (const char const *, const int, const int);
void input (const player, char *, const int, const int);
bool checkWin (const player, const char const *, const int, const int);
void switchPlayer (player *);   //function to change player
void handleError (const errors);	 //funtion to print approppriate error messages
void handleWin (const player);	//logic to run at the win of a player

int main (void)
{
    player nowPlaying;
    int ROWS, COLS;
	int cellsLeft;
	char ans;
	bool tie = 0;

	printf ("Hello! Welcome to this game of tic-tac-toe\n"
			"Here are the rules:\n");
	printRules ();

	printf ("Press Enter to continue");
	clearBuffer ();
	
	printf ("Enter the length of the side of the board (fixed for each session):\n");
	scanf ("%d", &ROWS);
	COLS = ROWS;

	char board[ROWS][COLS];

playAgain:
	system ("clear");
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			board[i][j] = ' ';

	cellsLeft = ROWS * COLS;
	nowPlaying = player2;

	do
	{
		switchPlayer (&nowPlaying);

		printBoard ((char *) board, ROWS, COLS);
		printf ("Enter the Indexes, %s\n", (nowPlaying == player1) ? "Player 1" : "Player 2");
		input (nowPlaying, (char *) board, ROWS, COLS);
		system ("clear");

		if (cellsLeft < 1)
		{
			printf ("TIE!\n");
			tie = 1;
			break;
		}
		cellsLeft--;
	} while (!checkWin (nowPlaying, (char *) board, ROWS, COLS));

	system ("clear");
	printBoard ((char *) board, ROWS, COLS);

	if (!tie)
		handleWin (nowPlaying);


	printf ("Do You Want To Play Again? (y or n)");
	if (getchar () != 'n')
	{
		clearBuffer ();
		goto playAgain;
	}
}


int clearBuffer (void)
{
	int i = 0, dump;

	while ((dump = getchar ()) != '\n'  &&  dump != EOF && dump != '\r')
		i++;

	return i;
}


void printBoard (char const *board, const int ROWS, const int COLS)
{
	printf ("  ");
	for (char i = 'A'; i < ROWS + 'A'; i++)
		printf (" %c  ", i);
	printf ("\n");
	
	for (int i = 0; i < ROWS; i++)
	{
		printf ("%d)", i);
		for (int j = 0; j < COLS; j++)
			printf (" %c %c", board[i * COLS + j], (j < COLS - 1) ? '\x7C' : '\n');

		if (i != ROWS - 1)
		{  
			printf ("  ");
			for (int j = 0; j < COLS; j++)
				printf ("----");
			printf("\n");
		}
	}
}


void input (const player nowPlaying, char *board, const int ROWS, const int COLS)
{
	char col;
	int row;

enterIndex:
	scanf ("%c%d", &col, &row);
	clearBuffer ();

	col = toupper(col) - 'A';  //convert to valid index

	if (row >= ROWS  ||  row < 0  ||  col >= COLS  ||  col < 0)
	{
		handleError (INVALID_INDEX);
		goto enterIndex;
	}

	if (board[row * COLS + col] != ' ')
	{
		handleError (USED_CELL);
		goto enterIndex;
	}

	switch (nowPlaying)
	{
		case player1:
			board[row * COLS + col] = 'X';
			break;

		case player2:
			board[row * COLS + col] = '0';
			break;

		default:
			handleError (PLAYER_ERROR);
			break;
	}
}


bool checkWin (const player nowPlaying, const char const *board, const int ROWS, const int COLS)
{
	//functions used only by checkWin ();
	bool checkRows (player, const char const *, const int, const int);
	bool checkDiags (player, const char const *, const int, const int);
	bool checkCols (player, const char const *, const int, const int);

	return (checkRows (nowPlaying, board, ROWS, COLS)  ||  checkCols (nowPlaying, board, ROWS, COLS)
                ||  checkDiags(nowPlaying, board, ROWS, COLS));
}


bool checkRows (const player nowPlaying, const char const *board, const int ROWS, const int COLS)
{
	const char charToSearch = (nowPlaying == player1) ? 'X' : '0';

	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (board[i * COLS + j] == charToSearch)
		    {
				if (j >= COLS - 1)
					return true;
            }

			else
				break;

	return false;
}


bool checkCols (const player nowPlaying, const char const *board, const int ROWS, const int COLS)
{
	char charToSearch;

	switch (nowPlaying)
	{
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

	for (int j = 0; j < COLS; j++)
		for (int i = 0; i < ROWS; i++)
			if (board[i * COLS + j] == charToSearch)
			{
				if (i >= ROWS - 1)
					return true;
			}
			else
				break;

	return false;
}


bool checkDiags (const player nowPlaying, const char const *board, const int ROWS, const int COLS)
{
	char charToSearch;

	switch (nowPlaying)
	{
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
	for (int i = 0, j = 0; i < ROWS  &&  j < COLS; i++, j++)
		if (board[i * COLS + j] == charToSearch)
		{
			if (i == ROWS - 1)
			{
				return true;
				printf ("DIAGONAL CHECKING WIN!\n");
			}
			continue;
		}
		else
			break;

	//check second diagonal
	for (int i = ROWS - 1, j = 0; i >= 0  &&  j < COLS; i--, j++)
		if (board[i * COLS + j] == charToSearch)
		{
			if (i == 0)
			{
				return true;
				printf ("DIAGONAL CHECKING WIN!\n");
			}
			continue;
		}
		else
			break;

	return false;
}


void switchPlayer (player *nowPlaying)
{
	switch (*nowPlaying)
	{
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
	switch (errorCode)
	{
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
}


void handleWin (const player winner)
{
	printf ("Congratularions %s! You Win!\n", (winner == player1) ? "Player 1" : "Player 2");
	printf ("I Hope You Enjoyed The Game!\n");
}





