#ifndef GAME_H
#define GAME_H
#include "chessboardgroup.h"

/*
 * Game class:
 *     include an array of chessboardgroup
 *
 * Objects:
 * chessgroup_:
 *     all chess that has been set in the game
 *
 * Methods:
 * Click(pos):
 *     if exists a chess C in pos:
 *         if drop_mode_ = QUANTUM:
 *             1. dropped_chess_group_.empty() && type(C) = QUANTUM:
 *                 last_entangled_chess_ = C
 *             2. return
 *         else:
 *             return
 *     else:
 *         DropChess(C)
 * FinishRound():
 *     true if dropped_chess_group_.size() = 1(CLASSIC) / 2(QUANTUM) / $(SUPERPO)
 * PrevRound():
 *     round.pop_back() && reset round.last()
 * NextRound():
 *     if FinishRound(), new ChessBoardGroup()
 */

class GameWindow;

class Game
{
public:
    Game(GameWindow *parent, const int &board_size, const TakeMode &takemode);
    ~Game();
    void Click(const coordinate &pos, const bool &right);
    bool FinishRound();
    void PrevRound();
    void NextRound();
private:
    GameWindow *parent_;
    const int board_size_;
    TakeMode takemode_;
    int current_round_id_;
    Player current_round_player_;
    std::vector<ChessBoardGroup *> round_;
    std::vector<Chess *> chessgroup_;
    friend class GameWindow;
};

#endif // GAME_H
