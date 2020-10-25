#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "chess.h"
#include <vector>

/*
 * ChessBoard class:
 *     A 2d-board, containing a square of chess(pointer), with weight(probability)
 *
 * Objects:
 * board_size_:
 *     count of chess in one line, default 19
 * board_weight_:
 *     represents of the chance to appear, default 1
 *
 * Methods:
 * ChessDead(x, y):
 *     true if the chess(pointer) board[x][y] is dead(has no qi)
 * ChessDead(pos):
 *     same as above
 * ChessMatch(chess):
 *     true if the dropped chess will appear in the chessboard in next turn
 */

class ChessBoardGroup;
class Game;
class GameWindow;

class ChessBoard {
private:
    const int board_size_;
    frac board_weight_;
    std::vector<std::vector<Chess *>> board_;
public:
    ChessBoard(const int &board_size, const frac &board_weight);
    ~ChessBoard();
    bool Captured(const int &x, const int &y) const;
    bool Captured(const coordinate &pos) const;
    bool Possess(const Chess &chess) const;
    friend class ChessBoardGroup;
    friend class Game;
    friend class GameWindow;
};

#endif // CHESSBOARD_H
