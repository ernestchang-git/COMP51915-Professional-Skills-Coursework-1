#include "Game.hpp"

#include <exception>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>

using Move = std::pair<int, int>;

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void play_all(Game& game, std::initializer_list<Move> moves) {
    for (const auto& move : moves) {
        check(game.place(move.first, move.second), "a valid move was rejected");
    }
}

void test_player_count_and_board_size() {
    bool rejected = false;
    try {
        Game invalid(1);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    check(rejected, "one player must be rejected");

    Game two_players(2);
    Game four_players(4);
    check(two_players.board_size() == 3, "two players need a 3x3 board");
    check(four_players.board_size() == 5, "four players need a 5x5 board");
    check(two_players.status() == GameStatus::InProgress, "a new game must be in progress");
}

void test_turns_and_invalid_moves() {
    Game game(2);
    check(game.current_player() == 1, "player 1 should start");
    check(!game.place(-1, 0), "outside-board click should be rejected");
    check(game.current_player() == 1, "invalid click must not use a turn");
    check(game.place(0, 0), "first valid move should succeed");
    check(game.cell(0, 0) == 1, "square should belong to player 1");
    check(game.current_player() == 2, "player 2 should play next");
    check(!game.place(0, 0), "occupied square should be rejected");
    check(game.current_player() == 2, "occupied-square click must not use a turn");
    check(game.place(1, 0), "player 2 should be able to move");
    check(game.cell(1, 0) == 2, "square should belong to player 2");
    check(game.current_player() == 1, "turns should cycle back to player 1");
}

void check_player_one_wins(std::initializer_list<Move> moves) {
    Game game(2);
    play_all(game, moves);
    check(game.status() == GameStatus::Won, "three in a row should end the game");
    check(game.winner() == 1, "player 1 should be the winner");
    check(!game.place(2, 2), "moves after a win should be rejected");
}

void test_horizontal_win() {
    check_player_one_wins({{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}});
}

void test_vertical_win() {
    check_player_one_wins({{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}});
}

void test_diagonal_wins() {
    check_player_one_wins({{0, 0}, {0, 1}, {1, 1}, {1, 0}, {2, 2}});
    check_player_one_wins({{0, 2}, {0, 0}, {1, 1}, {1, 0}, {2, 0}});
}

void test_three_in_a_row_on_larger_board() {
    Game game(4);
    play_all(game, {{2, 1}, {0, 0}, {0, 1}, {0, 2},
                    {2, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 3}});
    check(game.status() == GameStatus::Won, "three marks should win on a 5x5 board");
    check(game.winner() == 1, "player 1 should win the four-player game");
}

void test_full_board_draw() {
    Game game(2);
    play_all(game, {{0, 0}, {0, 1}, {0, 2},
                    {1, 1}, {1, 0}, {1, 2},
                    {2, 1}, {2, 0}, {2, 2}});
    check(game.status() == GameStatus::Draw, "full board without a winner is a draw");
    check(game.winner() == 0, "a draw must have no winner");
}

int main() {
    struct Test { const char* name; void (*run)(); };
    const Test tests[] = {
        {"player count and board size", test_player_count_and_board_size},
        {"turns and invalid moves", test_turns_and_invalid_moves},
        {"horizontal win", test_horizontal_win},
        {"vertical win", test_vertical_win},
        {"diagonal wins", test_diagonal_wins},
        {"three in a row on larger board", test_three_in_a_row_on_larger_board},
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
