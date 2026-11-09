# Tic-Tac-Two

Tic-Tac-Two is our terminal-based generalised version of a famous game Tic-Tac-Toe.
Players (from 3 to 10 people) take turns placing their symbols on a square grid,
and the first player to line up 3 of their symbols in a row wins.

## Authors
Group 4 of Professional Skills (26/27) Collaborative Software Development:
1. Ernest Chang
2. Linjie Mo
3. Renata Quinones Busquets
4. Alisa Sakva

## Game Overview

### Players and board size
The game supports **N players**, 3 ≤ N ≤ 10.
The board is a square grid of size **(N + 1) × (N + 1)**, so it grows with the number of players:

| N | Board size |
|-------------|------------|
| 3  | 4 × 4   |
| 4  | 5 × 5   |
| 5  | 6 × 6   |
| 6  | 7 × 7   |
| 7  | 8 × 8   |
| 8  | 9 × 9   |
| 9  | 10 × 10 |
| 10 | 11 × 11 |

### Player symbols
Each player has their own symbol. Symbols are assigned in the following order:

| Player | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|--------|---|---|---|---|---|---|---|---|---|----|
| Symbol | `X` | `O` | `*` | `#` | `@` | `+` | `%` | `&` | `$` | `!` |

## How to Play:

1. **Choose the number of players**, the number must be between 3 and 10.
2. **Take turns**, player 1 moves first, then player 2, and so on.
3. **Make a move**, on your turn, enter the row and the column of an empty cell, separated by a space.
   Rows and columns are numbered from 1. For example, to place your symbol in row 2, column 3, type:
   ```
   2 3
   ```
4. The board, with row and column numbers, is **printed after every move**.
5. If the chosen cell is already taken, lies outside the board,
   or the input is not two numbers, **the move is rejected** and the same player is asked to try again.

## Win Conditions:

- A player wins as soon as **3 of their symbols are in a row**, with no gaps, in any of these directions:
  - horizontally,
  - vertically,
  - diagonally, in either direction.
- A line longer than three (for example, 4 in a row) also counts as a win, because it contains three in a row.
- The board is checked **after every single move**. The game ends immediately when a player completes
  a line of three, and that player is declared the winner.

*Notes on getting a draw:* if every cell of the board is filled and nobody has three in a row, the game ends in a draw. Of course there are other instances of getting a draw in the game of tic-tac-toe, however for the purposes of this course we did not find it necessary to implement those cases, since it would take a lot of time.

### Example
Let's say we havea 3-player game, the board would be a 4 × 4 grid. Player 1 (`X`) has just completed a diagonal
from (1, 1) to (3, 3) and wins:

```
    1   2   3   4
1   X   O   *   .
2   .   X   O   .
3   *   .   X   .
4   .   .   .   .
```

*This shows the winning pattern only; the exact on-screen layout of the board may differ.*

## Build Instructions:

[to fill this part when we actually do the build part]

## Testing Instructions:

[to fill this part when we actually do the testing part]