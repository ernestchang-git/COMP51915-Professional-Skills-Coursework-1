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
   * Win condition: the first player to get 3 in a row, column or diagonal wins.
   * If player p in [1,...,P] has won, then you should return p; otherwise 0.
   */
  // Directions to check: row, column, diagonal down-right, diagonal down-left.
  const int dm[4] = {0, 1, 1, 1};
  const int dn[4] = {1, 0, 1, -1};

  for (int m = 0; m < M; m++) {
    for (int n = 0; n < N; n++) {
      int p = board[m][n];
      if (p == 0) continue;

      for (int d = 0; d < 4; d++) {
        int m2 = m + dm[d], n2 = n + dn[d];
        int m3 = m + 2 * dm[d], n3 = n + 2 * dn[d];
        if (m3 >= 0 && m3 < M && n3 >= 0 && n3 < N &&
            board[m2][n2] == p && board[m3][n3] == p) {
          return p;
        }
      }
    }
  }
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

int main(int argc, char *argv[]) {
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