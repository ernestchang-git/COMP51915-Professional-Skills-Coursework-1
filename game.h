#ifndef GAME_H
#define GAME_H

const int MIN_PLAYERS = 3;
const int MAX_PLAYERS = 10;
const int MAX_SIZE = MAX_PLAYERS + 1;
const char SYMBOLS[MAX_PLAYERS] = {'X', 'O', '*', '#', '@', '+', '%', '&', '$', '~'};

void initBoard(int board[MAX_SIZE][MAX_SIZE], int M, int N);
int parseBoard(int board[MAX_SIZE][MAX_SIZE], int M, int N);
bool isBoardFull(int board[MAX_SIZE][MAX_SIZE], int M, int N);
void printBoard(int board[MAX_SIZE][MAX_SIZE], int M, int N);
int gameLoop(int board[MAX_SIZE][MAX_SIZE], int players, int M, int N);
int readPlayerCount();
void printScores(int scores[], int players);
int runGame();

#endif
