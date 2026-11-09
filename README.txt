# Multiplayer Tic-Tac-Toe

## Overview

This is a multiplayer version of tic-tac-toe. At the start of the game, players enter the number of participants, \(n\). The program then creates an \((n+1)\times(n+1)\) board. At least two players are required, and there is no time limit.

Each player is assigned a different character as their mark. Players take turns clicking squares on the board. The first player to place three of their marks in a row wins.

## Game Rules

1. The number of players must be at least two. Once the number is entered, the program creates the board and assigns a distinct mark to each player.
2. Players take turns in the order they joined. On each turn, a player may click one empty square to place one mark.
3. A player wins immediately when **three consecutive marks** of their own form a horizontal, vertical, or diagonal line.
4. If the board is full and nobody has won, the game ends in a draw.
5. If a player clicks an occupied square or makes another invalid move, the program displays a text message and does not make the move. The same player then chooses again.

## Interface

Players select squares by clicking the board. The program displays text messages when the game starts, when a move is invalid, and when the game ends. The final message identifies the winner or announces a draw.

## Technology and Running the Game

The project is written in **C++**. Compiler requirements, build commands, and instructions for starting the game will be added once the code structure has been decided.

## Testing

The project needs tests for the minimum number of players, board size, turn order, invalid moves, winning lines in all three directions, and a draw when the board is full. Test files and commands will be added once the implementation has been decided.

## Continuous Integration

The continuous integration (CI) configuration and automated checks will be documented once they have been implemented.