#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "munit.h"
#include "game.h"

using Move = std::pair<int, int>;  // one-based row and column

struct IOCapture {
  explicit IOCapture(const std::string& text) : input(text) {
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

static bool contains(const std::string& text, const std::string& part) {
  return text.find(part) != std::string::npos;
}

static MunitResult test_player_count_and_symbols(const MunitParameter params[],
                                                  void* user_data) {
  (void)params;
  (void)user_data;
  munit_assert_int(MIN_PLAYERS, ==, 3);
  munit_assert_int(MAX_PLAYERS, ==, 10);
  munit_assert_string_equal(std::string(SYMBOLS, MAX_PLAYERS).c_str(),
                            "XO*#@+%&$!");

  int accepted_count;
  std::string message;
  {
    IOCapture io("2\n11\nhello\n3\n");
    accepted_count = readPlayerCount();
    message = io.output.str();
  }
  munit_assert_int(accepted_count, ==, 3);
  munit_assert_true(contains(message, "Invalid number of players!"));

  {
    IOCapture io("10\n");
    accepted_count = readPlayerCount();
  }
  munit_assert_int(accepted_count, ==, 10);

  {
    IOCapture io("");
    accepted_count = readPlayerCount();
  }
  munit_assert_int(accepted_count, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_board_initialisation_and_printing(
    const MunitParameter params[], void* user_data) {
  (void)params;
  (void)user_data;
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      munit_assert_int(board[row][column], ==, 0);
    }
  }

  board[0][0] = 1;
  board[1][2] = 2;
  std::string printed;
  {
    IOCapture io("");
    printBoard(board, 4, 4);
    printed = io.output.str();
  }
  munit_assert_true(contains(printed, "X"));
  munit_assert_true(contains(printed, "O"));
  munit_assert_int(std::count(printed.begin(), printed.end(), '.'), ==, 14);
  return MUNIT_OK;
}

static void assert_winner(int player, std::initializer_list<Move> cells) {
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  for (const auto& cell : cells) {
    board[cell.first - 1][cell.second - 1] = player;
  }
  munit_assert_int(parseBoard(board, 4, 4), ==, player);
}

static MunitResult test_winning_lines(const MunitParameter params[],
                                      void* user_data) {
  (void)params;
  (void)user_data;
  assert_winner(1, {{1, 1}, {1, 2}, {1, 3}});  // horizontal
  assert_winner(2, {{1, 4}, {2, 4}, {3, 4}});  // vertical
  assert_winner(3, {{1, 1}, {2, 2}, {3, 3}});  // diagonal down-right
  assert_winner(1, {{1, 4}, {2, 3}, {3, 2}});  // diagonal down-left
  assert_winner(2, {{2, 1}, {2, 2}, {2, 3}, {2, 4}});

  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  board[0][0] = 1;
  board[0][1] = 1;
  board[0][3] = 1;
  munit_assert_int(parseBoard(board, 4, 4), ==, 0);
  return MUNIT_OK;
}

static MunitResult test_board_full(const MunitParameter params[],
                                   void* user_data) {
  (void)params;
  (void)user_data;
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  munit_assert_false(isBoardFull(board, 4, 4));
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      board[row][column] = 1;
    }
  }
  munit_assert_true(isBoardFull(board, 4, 4));
  board[3][3] = 0;
  munit_assert_false(isBoardFull(board, 4, 4));
  return MUNIT_OK;
}

static MunitResult test_invalid_moves_and_immediate_win(
    const MunitParameter params[], void* user_data) {
  (void)params;
  (void)user_data;
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  int result;
  std::string printed;
  {
    IOCapture io("0 1\nhello\n1 1\n1 1\n2 1\n3 1\n"
                 "1 2\n2 2\n3 2\n1 3\n");
    result = gameLoop(board, 3, 4, 4);
    printed = io.output.str();
  }
  munit_assert_int(result, ==, 1);
  munit_assert_int(board[0][0], ==, 1);
  munit_assert_int(board[0][1], ==, 1);
  munit_assert_int(board[0][2], ==, 1);
  munit_assert_int(board[1][0], ==, 2);
  munit_assert_int(board[2][0], ==, 3);
  munit_assert_true(contains(printed, "Invalid input!"));
  munit_assert_true(contains(printed, "Illegal move!"));
  munit_assert_true(contains(printed, "Player 1 (X) has won!"));

  initBoard(board, 4, 4);
  {
    IOCapture io("");
    result = gameLoop(board, 3, 4, 4);
  }
  munit_assert_int(result, ==, 0);
  return MUNIT_OK;
}

static MunitResult test_full_board_draw(const MunitParameter params[],
                                        void* user_data) {
  (void)params;
  (void)user_data;
  int board[MAX_SIZE][MAX_SIZE];
  initBoard(board, 4, 4);
  int result;
  std::string printed;
  {
    IOCapture io("1 3\n1 1\n2 1\n3 3\n3 4\n3 1\n4 3\n4 1\n"
                 "2 3\n2 2\n4 2\n4 4\n2 4\n3 2\n1 2\n1 4\n");
    result = gameLoop(board, 3, 4, 4);
    printed = io.output.str();
  }
  munit_assert_int(result, ==, -1);
  munit_assert_true(isBoardFull(board, 4, 4));
  munit_assert_int(parseBoard(board, 4, 4), ==, 0);
  munit_assert_true(contains(printed, "It's a draw!"));
  return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/player-count-and-symbols", test_player_count_and_symbols, NULL, NULL,
     MUNIT_TEST_OPTION_NONE, NULL},
    {"/board-initialisation-and-printing", test_board_initialisation_and_printing,
     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {"/winning-lines", test_winning_lines, NULL, NULL, MUNIT_TEST_OPTION_NONE,
     NULL},
    {"/board-full", test_board_full, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {"/invalid-moves-and-immediate-win", test_invalid_moves_and_immediate_win,
     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {"/full-board-draw", test_full_board_draw, NULL, NULL,
     MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite suite = {
    "/tic-tac-two", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

int main(int argc, char* argv[]) {
  return munit_suite_main(&suite, NULL, argc, argv);
}
