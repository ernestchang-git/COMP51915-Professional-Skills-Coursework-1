// Test draft based on the team's Tic-Tac-Two README.
// This file assumes a Game.hpp interface with:
//   Game(int players)                 valid player counts: 3 through 10
//   int board_size() const            returns players + 1
//   int current_player() const        player numbers: 1 through players
//   bool place(int row, int column)   one-based coordinates; false for invalid moves
//   int cell(int row, int column)     0 if empty, otherwise player number
//   GameStatus status() const         InProgress, Won, or Draw
//   int winner() const                0 unless someone has won
// Adapt these calls to the real game API before adding this file to the team repo.

#include "Game.hpp"

#include <exception>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>

using Move = std::pair<int, int>;

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void play_all(Game& game, std::initializer_list<Move> moves) {
    for (const auto& [row, column] : moves) {
        check(game.place(row, column), "a valid move was rejected");
    }
}

void test_board_sizes_and_turn_order() {
    Game three(3);
    Game ten(10);
    check(three.board_size() == 4, "3 players need a 4x4 board");
    check(ten.board_size() == 11, "10 players need an 11x11 board");
    check(three.status() == GameStatus::InProgress, "new game must be in progress");
    check(three.current_player() == 1, "player 1 starts");

    check(three.place(1, 1), "player 1 should be able to move");
    check(three.cell(1, 1) == 1, "player 1 should own the square");
    check(three.current_player() == 2, "player 2 should follow player 1");
    check(three.place(2, 1), "player 2 should be able to move");
    check(three.current_player() == 3, "player 3 should follow player 2");
    check(three.place(3, 1), "player 3 should be able to move");
    check(three.current_player() == 1, "turns should cycle back to player 1");
}

void test_invalid_moves_do_not_use_a_turn() {
    Game game(3);
    check(!game.place(0, 1), "row zero must be rejected");
    check(!game.place(1, 0), "column zero must be rejected");
    check(!game.place(5, 1), "row beyond the 4x4 board must be rejected");
    check(game.current_player() == 1, "invalid coordinates must not use a turn");
    check(game.place(1, 1), "valid move must succeed");
    check(!game.place(1, 1), "occupied square must be rejected");
    check(game.current_player() == 2, "occupied square must not use player 2's turn");
}

void check_player_one_wins(std::initializer_list<Move> moves) {
    Game game(3);
    play_all(game, moves);
    check(game.status() == GameStatus::Won, "three in a row must end the game");
    check(game.winner() == 1, "player 1 must be the winner");
    check(!game.place(4, 4), "moves after the game ends must be rejected");
}

void test_horizontal_win() {
    check_player_one_wins({{1, 1}, {2, 1}, {3, 1},
                           {1, 2}, {2, 2}, {3, 2}, {1, 3}});
}

void test_vertical_win() {
    check_player_one_wins({{1, 1}, {1, 2}, {1, 3},
                           {2, 1}, {2, 2}, {2, 3}, {3, 1}});
}

void test_both_diagonal_wins() {
    check_player_one_wins({{1, 1}, {1, 2}, {1, 3},
                           {2, 2}, {2, 1}, {2, 3}, {3, 3}});
    check_player_one_wins({{1, 3}, {1, 1}, {1, 2},
                           {2, 2}, {2, 1}, {2, 3}, {3, 1}});
}

void test_three_in_a_row_inside_larger_board() {
    Game game(4);
    play_all(game, {{3, 2}, {1, 1}, {1, 2}, {1, 3},
                    {3, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 4}});
    check(game.status() == GameStatus::Won, "three marks win on a 5x5 board");
    check(game.winner() == 1, "player 1 must win");
}

void test_gap_filled_to_make_four_in_a_row() {
    Game game(4);
    play_all(game, {{3, 1}, {1, 1}, {2, 1}, {4, 1},
                    {3, 2}, {1, 2}, {2, 2}, {4, 2},
                    {3, 4}, {1, 4}, {2, 4}, {4, 4}});
    check(game.status() == GameStatus::InProgress,
          "a gap in the line must prevent an early win");
    check(game.place(3, 3), "filling the gap must be allowed");
    check(game.status() == GameStatus::Won,
          "a four-mark line also contains a winning three-mark line");
    check(game.winner() == 1, "player 1 must win");
}

void test_full_board_draw() {
    Game game(3);
    // The final 4x4 board contains no three consecutive marks for any player.
    play_all(game, {{1, 3}, {1, 1}, {2, 1}, {3, 3},
                    {3, 4}, {3, 1}, {4, 3}, {4, 1},
                    {2, 3}, {2, 2}, {4, 2}, {4, 4},
                    {2, 4}, {3, 2}, {1, 2}, {1, 4}});
    check(game.status() == GameStatus::Draw,
          "full board with no winner must be a draw");
    check(game.winner() == 0, "draw must have no winner");
}

int main() {
    struct Test { const char* name; void (*run)(); };
    const Test tests[] = {
        {"board sizes and turn order", test_board_sizes_and_turn_order},
        {"invalid moves", test_invalid_moves_do_not_use_a_turn},
        {"horizontal win", test_horizontal_win},
        {"vertical win", test_vertical_win},
        {"both diagonal wins", test_both_diagonal_wins},
        {"three in a row on larger board", test_three_in_a_row_inside_larger_board},
        {"four-mark line", test_gap_filled_to_make_four_in_a_row},
        {"full-board draw", test_full_board_draw},
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
