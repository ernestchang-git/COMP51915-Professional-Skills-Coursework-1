#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const int MIN_PLAYERS = 3;
const int MAX_PLAYERS = 10;
const int MAX_SIZE = MAX_PLAYERS + 1;
const char SYMBOLS[MAX_PLAYERS] = {'X', 'O', '*', '#', '@', '+', '%', '&', '$', '!'};

void initBoard(int board[MAX_SIZE][MAX_SIZE], int M, int N) {
  for (int m = 0; m < M; m++) {
    for (int n = 0; n < N; n++) {
      board[m][n] = 0;
    }
  }
}

int parseBoard(int board[MAX_SIZE][MAX_SIZE], int M, int N) {
  /*
   * Returns the player p in [1,...,P] who has 3 of their symbols in a row,
   * column or diagonal, or 0 if nobody has.
   * The board is checked after every single move, so at most one player can
   * ever have a line of 3 here: whoever completed it first is the winner.
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

bool isBoardFull(int board[MAX_SIZE][MAX_SIZE], int M, int N) {
  for (int m = 0; m < M; m++) {
    for (int n = 0; n < N; n++) {
      if (board[m][n] == 0) return false;
    }
  }
  return true;
}

void printBoard(int board[MAX_SIZE][MAX_SIZE], int M, int N) {
  /*
   * Prints the current board state as an M x N grid with row and column
   * numbers (starting from 1). Prints grid lines between each grid entry.
   * Each cell shows the symbol of the player who owns it, or '.' if it is 
   * empty.
   */
// Column numbers
  std::cout << "    ";
  for (int n = 0; n < N; n++) {
    std::cout << n + 1 << "   ";
  }
  std::cout << std::endl;

  for (int m = 0; m < M; m++) {

    // Horizontal separator
    std::cout << "   ";
    for (int n = 0; n < N; n++) {
      std::cout << "+---";
    }
    std::cout << "+" << std::endl;

    // Row contents
    std::cout << std::setw(2) << m + 1 << " ";

    for (int n = 0; n < N; n++) {

      char cell =
          (board[m][n] == 0)
              ? '.'
              : SYMBOLS[board[m][n] - 1];

      std::cout << "| " << cell << " ";
    }

    std::cout << "|" << std::endl;
  }

  // Bottom border
  std::cout << "   ";
  for (int n = 0; n < N; n++) {
    std::cout << "+---";
  }
  std::cout << "+" << std::endl;
}

int gameLoop(int board[MAX_SIZE][MAX_SIZE], int players, int M, int N) {
  /*
   * Runs the game until someone wins or the board is full (returns true),
   * or until input runs out mid-game (returns false).
   */
  std::string line;
  int p = 1;
  while (true) {
    std::cout << "Player " << p << " (" << SYMBOLS[p - 1] << "), enter row and column: ";
    if (!std::getline(std::cin, line)) {
      std::cout << std::endl << "No more input. Game aborted." << std::endl;
      return 0;
    }

    std::istringstream input(line);
    int m, n;
    std::string extra;
    if (!(input >> m >> n) || (input >> extra)) {
      std::cout << "Invalid input! Enter a row and a column, e.g. 2 3." << std::endl;
      continue;
    }
    if (m < 1 || m > M || n < 1 || n > N) {
      std::cout << "Illegal move! Row must be between 1 and " << M
                << ", column between 1 and " << N << "." << std::endl;
      continue;
    }
    if (board[m - 1][n - 1] != 0) {
      std::cout << "Illegal move! Cell (" << m << ", " << n << ") is already taken." << std::endl;
      continue;
    }

    board[m - 1][n - 1] = p;
    std::cout << SYMBOLS[p - 1] << " at (" << m << ", " << n << ")." << std::endl;
    printBoard(board, M, N);

    int winner = parseBoard(board, M, N);
    if (winner != 0) {
      std::cout << "Player " << winner << " (" << SYMBOLS[winner - 1] << ") has won!" << std::endl;
      return winner;
    }
    if (isBoardFull(board, M, N)) {
      std::cout << "The board is full. It's a draw!" << std::endl;
      return -1;
    }

    p = p % players + 1;
  }
}

int readPlayerCount() {
  std::string line;
  while (true) {
    std::cout << "Initialising game... How many players (" << MIN_PLAYERS << "-" << MAX_PLAYERS << ")? ";
    if (!std::getline(std::cin, line)) {
      std::cout << std::endl << "No more input. Game aborted." << std::endl;
      return 0;
    }
    std::istringstream input(line);
    int P;
    std::string extra;
    if ((input >> P) && !(input >> extra) && P >= MIN_PLAYERS && P <= MAX_PLAYERS) {
      return P;
    }
    std::cout << "Invalid number of players! Enter a number between " << MIN_PLAYERS
              << " and " << MAX_PLAYERS << "." << std::endl;
  }
}

void printScores(int scores[], int players) {

  std::cout << "\n=== SCOREBOARD ===\n";

  for (int p = 1; p <= players; p++) {
    std::cout
        << "Player "
        << p
        << " (" << SYMBOLS[p - 1] << ") : "
        << scores[p - 1]
        << " wins\n";
  }

  std::cout << std::endl;
}

int main() {
  int P = readPlayerCount();
  int scores[MAX_PLAYERS] = {0};
  if (P == 0) {
    return EXIT_FAILURE;
  }
  int M = P + 1;
  int N = P + 1;

  while (true) {
    int board[MAX_SIZE][MAX_SIZE];
    initBoard(board, M, N);
    printBoard(board, M, N);

    int result = gameLoop(board, P, M, N);
    if (result == 0) {
      return EXIT_FAILURE;
    }

    if (result > 0) {
      scores[result - 1]++;
    }

    printScores(scores, P);

    std::string answer;
    std::cout << "Play again? (y/n): ";
    std::getline(std::cin, answer);

    if (answer != "y" && answer != "Y") {
      break;
    }
  }

  return EXIT_SUCCESS;
}
