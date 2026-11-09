#include <algorithm>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using Move = std::pair<int, int>; // one-based row and column

void check(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}

struct IOCapture {
  explicit IOCapture(std::string text) : input(std::move(text)) {
    old_input = std::cin.rdbuf(input.rdbuf());
    old_output = std::cout.rdbuf(output.rdbuf());
    std::cin.clear();
  }

  ~IOCapture() {
    std::cin.rdbuf(old_input);
    std::cout.rdbuf(old_output);
    std::cin.clear();
  }

  std::istringstream input;
  std::ostringstream output;
  std::streambuf* old_input;
  std::streambuf* old_output;
};

void test_player_count_and_symbols() {
  check(MIN_PLAYERS == 3 && MAX_PLAYERS == 10, "player limits must be 3 to 10");
  check(std::string(SYMBOLS, MAX_PLAYERS) == "XO*#@+%&$!",
        "player symbols must match the README");
  {
    IOCapture io("2\n11\nhello\n3\n");
    check(readPlayerCount() == 3, "invalid counts must be retried");
    check(io.output.str().find("Invalid number of players!") != std::string::npos,
          "invalid count must show a message");
  }
  {
    IOCapture io("10\n");
    check(readPlayerCount() == 10, "ten players must be accepted");
  }
  {
    IOCapture io("");
    check(readPlayerCount() == 0, "EOF must abort player selection");
  }
}

void test_board_initialisation_and_printing() {
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      check(board[row][column] == 0, "new board must be empty");
    }
  }
  board[0][0] = 1;
  board[1][2] = 2;
  {
    IOCapture io("");
    printBoard(board, 4, 4);
    const std::string printed = io.output.str();
    check(printed.find('X') != std::string::npos,
          "board must display player 1's symbol");
    check(printed.find('O') != std::string::npos,
          "board must display player 2's symbol");
    check(std::count(printed.begin(), printed.end(), '.') == 14,
          "empty squares must be displayed as dots");
  }
}

void expect_winner(int player, std::initializer_list<Move> cells) {
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  for (const auto& [row, column] : cells) board[row - 1][column - 1] = player;
  check(parseBoard(board, 4, 4) == player, "winning line was not detected");
}

void test_winning_lines() {
  expect_winner(1, {{1, 1}, {1, 2}, {1, 3}}); // horizontal
  expect_winner(2, {{1, 4}, {2, 4}, {3, 4}}); // vertical
  expect_winner(3, {{1, 1}, {2, 2}, {3, 3}}); // down-right diagonal
  expect_winner(1, {{1, 4}, {2, 3}, {3, 2}}); // down-left diagonal
  expect_winner(2, {{2, 1}, {2, 2}, {2, 3}, {2, 4}}); // four includes three

  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  board[0][0] = 1;
  board[0][1] = 1;
  board[0][3] = 1;
  check(parseBoard(board, 4, 4) == 0, "a gap must prevent a win");
}

void test_board_full() {
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  check(!isBoardFull(board, 4, 4), "new board must not be full");
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) board[row][column] = 1;
  }
  check(isBoardFull(board, 4, 4), "filled board must be full");
  board[3][3] = 0;
  check(!isBoardFull(board, 4, 4), "one empty square prevents a full board");
}

void test_invalid_moves_and_immediate_win() {
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  {
    IOCapture io("0 1\nhello\n1 1\n1 1\n2 1\n3 1\n"
                 "1 2\n2 2\n3 2\n1 3\n");
    check(gameLoop(board, 3, 4, 4), "game must finish after player 1 wins");
    check(board[0][0] == 1 && board[0][1] == 1 && board[0][2] == 1,
          "player 1's winning moves must be placed");
    check(board[1][0] == 2 && board[2][0] == 3,
          "invalid moves must not change the turn order");
    const std::string printed = io.output.str();
    check(printed.find("Invalid input!") != std::string::npos,
          "non-numeric input must be rejected");
    check(printed.find("Illegal move!") != std::string::npos,
          "occupied and out-of-range moves must be rejected");
    check(printed.find("Player 1 (X) has won!") != std::string::npos,
          "win must be announced immediately");
  }
  initBoard(board, 4, 4);
  {
    IOCapture io("");
    check(!gameLoop(board, 3, 4, 4), "EOF must abort an unfinished game");
  }
}

void test_full_board_draw() {
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  {
    IOCapture io("1 3\n1 1\n2 1\n3 3\n3 4\n3 1\n4 3\n4 1\n"
                 "2 3\n2 2\n4 2\n4 4\n2 4\n3 2\n1 2\n1 4\n");
    check(gameLoop(board, 3, 4, 4), "full-board draw must end the game");
    check(isBoardFull(board, 4, 4), "draw must fill the board");
    check(parseBoard(board, 4, 4) == 0, "draw must have no winner");
    check(io.output.str().find("It's a draw!") != std::string::npos,
          "draw must be announced");
  }
}

void test_complete_program() {
  IOCapture io("3\n1 1\n2 1\n3 1\n1 2\n2 2\n3 2\n1 3\n");
  check(tic_tac_two_program_main() == EXIT_SUCCESS,
        "complete game must exit successfully after a win");
  check(io.output.str().find("Player 1 (X) has won!") != std::string::npos,
        "complete program must announce its winner");
}

int main() {
  struct Test { const char* name; void (*run)(); };
  const Test tests[] = {
      {"player count and symbols", test_player_count_and_symbols},
      {"board initialisation and printing", test_board_initialisation_and_printing},
      {"winning lines", test_winning_lines},
      {"board full", test_board_full},
      {"invalid moves and immediate win", test_invalid_moves_and_immediate_win},
      {"full-board draw", test_full_board_draw},
      {"complete program", test_complete_program},
  };

  int failures = 0;
  for (const Test& test : tests) {
    try {
      test.run();
      std::cout << "PASS: " << test.name << '\n';
    } catch (const std::exception& error) {
      ++failures;
      std::cerr << "FAIL: " << test.name << " - " << error.what() << '\n';
    }
  }
  return failures == 0 ? 0 : 1;
}
