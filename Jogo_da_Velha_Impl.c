#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void showBoard(const char board[3][3]);
int fillSquare(int index, char character, char board[3][3]);
int fillEmpty(int index, char character, char board[3][3]);
int countNumEmpty(char board[3][3]);
void firstToPlay();
int checkWinner(char board[3][3]);
int miniMaxAB(char board[3][3], int depth, int alpha, int beta, int isMaximizer);
void chooseSymbol();
void playTurn(char sPlayer, char board[3][3]);
void computerTurn(char board[3][3]);

char board[3][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}};

char boardLines[] = "   |   |   ";
void showBoard(const char board[3][3])
{
    for (int i = 0; i < 3; ++i)
    {
        printf("%s\n", boardLines);
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        printf("%s\n", boardLines);
        if (i < 2)
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

// Fills the nth space, returns index of filled square
int fillEmpty(int index, char character, char board[3][3])
{
    int counting = 0;
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            if (board[x][y] != 'X' && board[x][y] != 'O'){
                if (counting == index){
                    int position = (3 * x) + y + 1;
                    fillSquare(position, character, board);
                    return position;
                }
                ++counting;
            }
}

int countNumEmpty(char board[3][3])
{
    int counting = 0;
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            if (board[x][y] != 'X' && board[x][y] != 'O')
                ++counting;
    return counting;
}

int firstPlayer = 0, stage = 0;
int player = 0, opponent = 1;
void firstToPlay()
{
    // X is 0, O is 1
    srand(time(NULL));
    int order = rand() % 2;
    firstPlayer = order;
    if (firstPlayer == player) stage = 1;
    else stage = 2;

    printf("\nDefinindo aleatoriamente o primeiro jogador...\n\n");
    printf("O primeiro jogador sera ");
    if (order)
        printf("O!\n\n");
    else
        printf("o X!\n\n");
}

int checkWinner(char board[3][3])
{
    // X is 0, O is 1, draw is 2 and if the game is still going is 3
    for (int i = 0; i < 3; ++i){
        // check horizontal
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i] == 'X' ? 0 : 1;
        // check vertical
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0] == 'X' ? 0 : 1;
    }

    // check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0] == 'X' ? 0 : 1;;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2] == 'X' ? 0 : 1;;

    // check draw
    for (int x = 0; x < 3; ++x){
        for (int y = 0; y < 3; ++y){
            if (board[x][y] != 'X' && board[x][y] != 'O');
                return 3; // (false) the game is still going;
        }
    }
    return 2; // means that it was a draw
}

//int count = 0;
char sPlayer = 'X', sOpponent = 'O';
int miniMaxAB(char board[3][3], int depth, int alpha, int beta, int isMaximizer)
{
    int bestIndex;
    int winner = checkWinner(board);
    if (winner != 3){ // The game has a draw or a winner
        if (winner == player){
            return -10 + depth;
        }
        if (winner == opponent){
            return 10 - depth;
        }
        else{
            return 0;
        }
    }
    if (isMaximizer){
        int maxEval = -INT_MAX;
        for(int i = 0; i < countNumEmpty(board); ++i){
            char boardCopy[3][3];
            memcpy(boardCopy, board, 9);
            int index = fillEmpty(i, sOpponent, boardCopy);
            int eval = miniMaxAB(boardCopy, depth + 1, alpha, beta, 0);
            if (eval > maxEval){
                maxEval = eval;
                bestIndex = index;
            }
            alpha = MAX(alpha, eval);
            if (beta <= alpha)
                break;
        }
        if (depth == 0)
            return bestIndex;
        return maxEval;
    }
    else{ //Minimize
        int minEval = INT_MAX;
        for(int i = 0; i < countNumEmpty(board); ++i){
            char boardCopy[3][3];
            memcpy(boardCopy, board, 9);
            int index = fillEmpty(i, sPlayer, boardCopy);
            int eval = miniMaxAB(boardCopy, depth + 1, alpha, beta, 1);
            minEval = MIN(eval, minEval);
            beta = MIN(eval, beta);
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

void chooseSymbol()
{
    // X is 0, O is 1
    char symbol = 'A';
    while (symbol != 'X' && symbol != 'O'){
        printf("Deseja ser X ou O? ");
        scanf("%c", &symbol);
        symbol = toupper(symbol);
        getchar();
    }

    if (symbol == 'O'){
        player = 1;
        sPlayer = 'O';
        opponent = 0;
        sOpponent = 'X';
    }
}

void playTurn(char sPlayer, char board[3][3])
{
    int result = 0, num = 0;
    while (result == 0){
        printf("\n\nDigite o numero da casa o qual deseja jogar o %c (1 ate 9) ", sPlayer);
        scanf("%d", &num);

        while (num < 1 || num > 9){
            printf("Numero invalido! Digite um numero de 1 a 9 ");
            scanf("%d", &num);
        }
        result = fillSquare(num, sPlayer, board);
        if (result == 0) printf("Essa casa ja esta preenchida!\n");
    }
}

void computerTurn(char board[3][3])
{
    printf("\n\nComputer turn\n");
    int i = miniMaxAB(board, 0, -INT_MAX, INT_MAX, 1);
    fillSquare(i, sOpponent, board);
    printf("Computador jogou um %c na casa %d\n\n", sOpponent, i);
}

int main()
{
    chooseSymbol();
    firstToPlay();

    printf("Tabuleiro:\n");
    showBoard(board);
    int isPlaying = 1;
    int winner;
    while (isPlaying == 1){
        if (stage == 1){
            playTurn(sPlayer, board);
            showBoard(board);
            winner = checkWinner(board);
            if (winner != 3) // The game has draw or winner
                isPlaying = 0;
            else
                stage = 2;
        }
        else if (stage == 2){
            computerTurn(board);
            showBoard(board);
            winner = checkWinner(board);
            if (winner != 3) // The game has draw or winner
                isPlaying = 0;
            else
                stage = 1;
        }
    }
    if (winner == 0)
        printf("\n\nO vencedor eh X!");
    else if (winner == 1)
        printf("\n\nO vencedor eh O!");
    else
        printf("\n\nDeu velha!");

    return 0;
}
