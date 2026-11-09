cmake_minimum_required(VERSION 3.20)
project(multiplayer_tic_tac_toe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(game_logic src/Game.cpp)
target_include_directories(game_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(game_tests tests/game_tests.cpp)
target_link_libraries(game_tests PRIVATE game_logic)

enable_testing()
add_test(NAME game_rules COMMAND game_tests)
