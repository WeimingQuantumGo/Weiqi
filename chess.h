#ifndef CHESS_H
#define CHESS_H
#include "point.h"

/* Chess class:
 *     consisting of 3 different types of chess: classic, superpo, entangle
 *
 * Objects:
 * id_:
 *     the drop order of the chess, start at 1
 * pos_:
 *     the relative coordinate of the chess, from (0, 0) to (size - 1, size - 1)
 * player_:
 *     the player of the chess
 * chess_weight:
 *     the probability to appear, 1 or 1/2
 *
 * Methods:
 * operator QString():
 *     id_
 */

enum Player {WHITE, BLACK};

enum DropMode {CLASSIC, SUPERPO, ENTANGLE, GAMEWISE, UNDEFINED};

class Chess {
private:
    int id_;
    Player player_;
    bool entangleable_;
public:
    Chess(const int &id, const bool &entangleable, const Player &player);
    int id() const;
    Player player() const;
    bool entangleable() const;
    void set_entangleable(const bool &entangleable);
    explicit operator QString() const;
};

class ChessWithPos {
public:
    Chess *chess_;
    coordinate pos_;
    ChessWithPos(Chess *chess = nullptr, const coordinate &pos = {-1, -1});
    bool operator<(const ChessWithPos &other) const;
};

#endif // CHESS_H
