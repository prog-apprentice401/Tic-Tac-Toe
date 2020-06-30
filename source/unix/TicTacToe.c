#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3

#define true 1
#define false 0

#define PLAYER_ERROR    0
#define USED_CELL       1
#define INVALID_INDEX   2

typedef _Bool bool;

typedef enum player
{
    player1 = 1,
    player2 = 2
} player;


/*function definitions for main*/

int clearBuffer (void);     //utility function to clear the buffer when needed
void printRules (void);
void printBoard (void);
void input (player);
bool checkWin (player);
void switchPlayer (player *);   //function to change player
void handleError (int);     //funtion to print approppriate error messages
void handleWin (player);    //logic to run at the win of a player
bool getAns (void);

char board[ROWS][COLS];


int main (void)
{
    player nowPlaying;
    int cellsLeft;
    char ans;
    bool tie = 0;

    printf ("Hello! Welcome to this game of tic-tac-toe\n"
            "Here are the rules:\n");
    printRules ();

    printf ("Press Enter to continue");
    clearBuffer ();

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

        printBoard ();
        printf ("Enter the Indexes, %s\n", (nowPlaying == player1) ? "Player 1" : "Player 2");
        input (nowPlaying);
        system ("clear");

        if (--cellsLeft < 1)
        {
            printf ("TIE!\n");
            tie = 1;
            break;
        }
    } while (!checkWin (nowPlaying));

    system ("clear");
    printBoard ();

    if (!tie)
        handleWin (nowPlaying);
    if (getAns ())
        goto playAgain;
}


int clearBuffer (void)
{
    int i = 0, dump;

    while ((dump = getchar ()) != '\n'  &&  dump != EOF)
        i++;

    return i;
}


void printRules (void)
{
    printf ("\nThis is a two player game.\n"
	        "2) Players take turns entering the row and column number\n"
	        "   of the cell they want their symbol to go into.\n"
	        "3) Player 1 is assigned 'X' and Player 2 is assigned '0'\n"
	        "4) The first player to align THREE of their own symbols\n"
	        "   in a row, column, or diagonal wins.\n\n\n");
}


void printBoard (void)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
            printf (" %c %c", board[i][j], (j < 2) ? '\x7C' : '\n');

        printf ("%s\n", (i < ROWS - 1) ? "-----------" : "");
    }
}


void input (player nowPlaying)
{
    char col;
    int row;

enterIndex:
    scanf ("%c%d", &col, &row);
    clearBuffer ();

    switch (col)
    {
        case 'A': case 'a':
            col = 0;
            break;

        case 'B': case 'b':
            col = 1;
            break;

        case 'C': case 'c':
            col = 2;
            break;

        default:
            handleError (INVALID_INDEX);
            goto enterIndex;
    }

    if (row > 2  ||  row < 0)
    {
        handleError (INVALID_INDEX);
        goto enterIndex;
    }

    if (board[row][col] != ' ')
    {
        handleError (USED_CELL);
        goto enterIndex;
    }

    switch (nowPlaying)
    {
        case player1:
            board[row][col] = 'X';
            break;

        case player2:
            board[row][col] = '0';
            break;

        default:
            handleError (PLAYER_ERROR);
            break;
    }
}


bool checkWin (player nowPlaying)
{
    //functions used only by checkWin ();
    bool checkRows (player);
    bool checkDiags (player);
    bool checkCols (player);

    return (checkRows (nowPlaying)  ||  checkCols (nowPlaying)  ||  checkDiags(nowPlaying));
}


bool checkRows (const player nowPlaying)
{
    const char charToSearch = (nowPlaying == player1) ? 'X' : '0';

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j] == charToSearch)
            {
                if (j >= COLS - 1)
                    return true;
            }

            else
                break;

    return false;
}


bool checkCols (const player nowPlaying)
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
            if (board[i][j] == charToSearch)
            {
                if (i >= ROWS - 1)
                    return true;
            }
            else
                break;

    return false;
}


bool checkDiags (const player nowPlaying)
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
        if (board[i][j] == charToSearch)
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
        if (board[i][j] == charToSearch)
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


void handleError (int errorCode)
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
                    "The Index Is Invalid\n\n");
            break;
    }
}


void handleWin (player winner)
{
    printf ("Congratularions %s! You Win!\n", (winner == player1) ? "Player 1" : "Player 2");
    printf ("I Hope You Enjoyed The Game!\n");
}


bool getAns (void)
{
    printf ("Do You Want To Play Again? (y or n)");
    char ans = getchar ();
    clearBuffer ();

    return (ans == 'n') ? 0 : 1;
}
