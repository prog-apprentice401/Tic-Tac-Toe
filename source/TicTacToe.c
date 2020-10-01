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
void printBoard (const char const *, const int);
void input (const player, char *, const int);
bool checkWin (const player, const char const *, const int);
void switchPlayer (player *);   //function to change player
void handleError (const errors);	 //funtion to print approppriate error messages
void handleWin (const player);	//logic to run at the win of a player

int main (void)
{
    player nowPlaying;
    int sideLength;
	int cellsLeft;
	char *board = NULL;
	char ans;
	bool tie = 0;
	system ("clear");
	printf ("Hello! Welcome to this game of tic-tac-toe\n"
			"Here are the rules:\n");
	printRules ();

	printf ("Press Enter to continue");
	clearBuffer ();
	
playAgain:
	system ("clear");

	while (1)
	{
		printf ("Enter the length of the side of the board:\n");
		scanf ("%d", &sideLength);
		clearBuffer ();
	
		if (sideLength <= 0 || sideLength > 26)
			printf ("Maximum Length is 26!\n");
		else
			break;
	}

	board = malloc (sideLength * sideLength);

	system ("clear");
	for (int i = 0; i < sideLength; i++)
		for (int j = 0; j < sideLength; j++)
			board[i * sideLength + j] = ' ';

	cellsLeft = sideLength * sideLength;
	nowPlaying = player2;

	do
	{
		switchPlayer (&nowPlaying);

		printBoard (board, sideLength);
		printf ("Enter the Indexes, %s\n", (nowPlaying == player1) ? "Player 1" : "Player 2");
		input (nowPlaying, board, sideLength);
		system ("clear");

		if (cellsLeft < 1)
		{
			printf ("TIE!\n");
			tie = 1;
			break;
		}
		cellsLeft--;
	} while (!checkWin (nowPlaying, board, sideLength));

	system ("clear");
	printBoard (board, sideLength);

	if (!tie)
		handleWin (nowPlaying);

	free (board);

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


void printBoard (char const *board, const int sideLength)
{
	printf ("   ");
	for (char i = 'A'; i < sideLength + 'A'; i++)
		printf (" %c  ", i);
	printf ("\n");
	
	for (int i = 0; i < sideLength; i++)
	{
		printf ("%2d)", i);
		for (int j = 0; j < sideLength; j++)
			printf (" %c %c", board[i * sideLength + j], (j < sideLength - 1) ? '\x7C' : '\n');

		if (i != sideLength - 1)
		{  
			printf ("  ");
			for (int j = 0; j < sideLength; j++)
				printf ("----");
			printf("\n");
		}
	}
}


void input (const player nowPlaying, char *board, const int sideLength)
{
	char col;
	int row;

enterIndex:
	scanf ("%c%d", &col, &row);
	clearBuffer ();

	col = toupper(col) - 'A';  //convert to valid index
	printf ("Index: %c%d\n", col + 'A', row);	
	if (row >= sideLength  ||  row < 0  ||  col >= sideLength  ||  col < 0)
	{
		printf ("Index: %c%d\n", col, row);
		handleError (INVALID_INDEX);
		goto enterIndex;
	}

	if (board[row * sideLength + col] != ' ')
	{
		printf ("Cell: %c%d\n", col, row);
		handleError (USED_CELL);
		goto enterIndex;
	}

	switch (nowPlaying)
	{
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


bool checkWin (const player nowPlaying, const char const *board,  const int sideLength)
{
	//functions used only by checkWin ();
	bool checkRows (player, const char const *, const int);
	bool checkDiags (player, const char const *, const int);
	bool checkCols (player, const char const *, const int);

	return (checkRows (nowPlaying, board, sideLength)  ||  checkCols (nowPlaying, board, sideLength)
                ||  checkDiags(nowPlaying, board, sideLength));
}


bool checkRows (const player nowPlaying, const char const *board, const int sideLength)
{
	const char charToSearch = (nowPlaying == player1) ? 'X' : '0';

	for (int i = 0; i < sideLength; i++)
		for (int j = 0; j < sideLength; j++)
			if (board[i * sideLength + j] == charToSearch)
		    {
				if (j >= sideLength - 1)
					return true;
            }

			else
				break;

	return false;
}


bool checkCols (const player nowPlaying, const char const *board, const int sideLength)
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

	for (int j = 0; j < sideLength; j++)
		for (int i = 0; i < sideLength; i++)
			if (board[i * sideLength + j] == charToSearch)
			{
				if (i >= sideLength - 1)
					return true;
			}
			else
				break;

	return false;
}


bool checkDiags (const player nowPlaying, const char const *board, const int sideLength)
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
	for (int i = 0, j = 0; i < sideLength  &&  j < sideLength; i++, j++)
		if (board[i * sideLength + j] == charToSearch)
		{
			if (i == sideLength - 1)
			{
				return true;
				printf ("DIAGONAL CHECKING WIN!\n");
			}
			continue;
		}
		else
			break;

	//check second diagonal
	for (int i = sideLength - 1, j = 0; i >= 0  &&  j < sideLength; i--, j++)
		if (board[i * sideLength + j] == charToSearch)
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





