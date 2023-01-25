# Hexapawn

Hexapawn is a simple game: it's played on a 3x3 chessboard, both white and black play with 3 pawns each.
The only legal moves are either a vertical one-square-push forward or a diagonal capture, like in chess.
Victory is achieved if:
- you leave your opponent with no legal moves
- you reach the opposite side of the board
Hexapawn is a solved game, meaning in each position it's possible to determine the outcome of the match, and with perfect play black always wins in 3 moves.

## Solving Hexapawn

The game can be easily solved with a minimax search, in this case with alpha-beta pruning.
In this code the engine will always play random moves for white and the optimal one for black.
For now, in case more than one winning move is found, only the first one is executed and the others are ignored.
A way to randomly select between the best ones could also be implemented, just to make games a bit more.