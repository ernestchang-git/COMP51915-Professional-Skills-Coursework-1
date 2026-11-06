#include <bits/stdc++.h>
#include <cstdio>
#include <cstdlib>

const int M = 1 + pow(2, 7);  // You can change this for your specific game!
const int N = 1 + pow(2, 3);  // You can change this for your specific game!

void initBoard(int board[M][N], int M, int N) {
  for (int m = 0; m < M; m++) {
    for (int n = 0; n < N; n++) {
      board[m][n] = 0;
    }
  }
}

int parseBoard(int board[M][N]) {
  /*
   * This function is where you should define your win conditions.
   * If player p in [1,...,P] has won, then you should return p; otherwise 0.
   * If you find yourself with a great deal of spare time you might try
   * rendering the board to the terminal here, too.
   */
  return 0;
}

void gameLoop(int board[M][N], bool cont, int players, int M, int N) {
  int winner;
  int m, n;
  do {
    for (int p = 1; p <= players; ++p) {
      std::cout << "Player " << p << " place:";
      std::cin >> m >> n;
      if (board[m][n] == 0) {
        std::cout << p << " at (" << m << ", " << n << ")." << std::endl;
        board[m][n] = p;
      } else {
        std::cout << "Illegal move!" << std::endl;
      }
    }
    winner = parseBoard(board);
    if (winner != 0) {
      std::cout << "Player " << winner << " has won!" << std::endl;
      cont = false;
    }
  } while (cont);
}

int main(void) {
  int P;
  bool cont = true;
  if (argc < 1) {
    return EXIT_FAILURE;
  } else {
    std::cout << "Initialising game... How many players?";
    std::cin >> P;
  }
  int board[M][N];
  initBoard(board, M, N);
  gameLoop(board, cont, P, M, N);
  return EXIT_SUCCESS;
}
