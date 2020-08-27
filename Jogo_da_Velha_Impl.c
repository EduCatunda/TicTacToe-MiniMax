#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 3

int firstToPlay();
void showBoard(const char board[3][3]);
int fillSquare(int index, char character, char board[3][3]);
void runGame();

// Definindo o tabuleiro
char board[MAX][MAX] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}};

char boardLines[] = "   |   |   ";
void showBoard(const char board[3][3])
{
    for (int i = 0; i < MAX; ++i)
    {
        printf("%s\n", boardLines);
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        printf("%s\n", boardLines);
        if (i < MAX - 1)
            printf("-----------\n");
    }
}

int fillSquare(int index, char character, char board[3][3])
{
    int row = (index - 1) / 3;
    int col = (index - 1) % 3;
    if (board[row][col] == 'X' || board[row][col] == 'O')
        return 0;
    board[row][col] = character;

    return 1;
}

int firstToPlay()
{
    srand(time(NULL));
    int order = rand() % 2;
    return order;
}

void runGame()
{
}

int main()
{

    int index;
    printf("Definindo aleatoriamente o primeiro jogador...\n");

    index = firstToPlay();
    printf("O jogador %d irá começar!\n\n", index + 1);

    printf("Tabuleiro:\n");
    showBoard(board);

    scanf("%d", &index);
    fillSquare(index, 'X', board);

    showBoard(board);
    return 0;
}
