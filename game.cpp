#include "game.h"

Game::Game(GameWindow *parent, const int &board_size, const TakeMode &takemode)
    : parent_(parent), board_size_(board_size), takemode_(takemode) {
    current_round_id_ = 0;
    current_round_player_ = Player::BLACK;
    round_.push_back(new ChessBoardGroup(board_size_));
}

Game::~Game() {
    for (auto group : round_)
        delete group;
    for (auto chess : chessgroup_)
        delete chess;
    round_.clear();
    chessgroup_.clear();
}

void Game::Click(const coordinate &pos, const bool &right) {
    auto round = round_[current_round_id_];
    if (right)
        round->RPress(pos);
    else {
        round->LPress(pos);
//        if (FinishRound())
//            NextRound();
    }
}

bool Game::FinishRound() {
    auto round = round_[current_round_id_];
    int current_board_gen = 0;
    for (auto dropped : round->dropped_chess_group_)
        for (int i = 0; i < 8; ++i) if ((dropped.first >> i) & 1) ++current_board_gen;
    qDebug() << "current_board_gen" << current_board_gen;
    qDebug() << "round->board_gen" << round->board_gen;
    return current_board_gen == round->board_gen;
}

void Game::PrevRound() {
    round_.pop_back();
    --current_round_id_;
    if (current_round_player_ == Player::WHITE)
        current_round_player_ = Player::BLACK;
    else
        current_round_player_ = Player::WHITE;
    round_.back()->reset();
}

void Game::NextRound() {
    round_.push_back(new ChessBoardGroup(*round_.back(), takemode_));
    ++current_round_id_;
    if (current_round_player_ == Player::WHITE)
        current_round_player_ = Player::BLACK;
    else
        current_round_player_ = Player::WHITE;
    qInfo(
                ("current round: "
                 + std::to_string(current_round_id_ + 1)
                 + ", player: "
                 + (current_round_player_ == Player::WHITE ? "WHITE" : "BLACK")
                 ).data()
                );
}
