#include "chessboardgroup.h"

ChessBoardGroup::ChessBoardGroup(const int &board_size, bool reverse)
    : board_size_(board_size), current_player_(static_cast<Player>(reverse ^ 1)),
      activate_chessboard_index_(0), drop_mode_(DropMode::UNDEFINED), board_gen(1), double_in_gamewise(0)
{
    chessboard_group_.push_back(new ChessBoard(board_size_, 1));
    new_chess = new Chess(1, false, current_player_);
}

ChessBoardGroup::ChessBoardGroup(const ChessBoardGroup &previous_group, const TakeMode &takemode)
    : board_size_(previous_group.board_size_), current_player_(static_cast<Player>(previous_group.current_player_ ^ 1)),
      activate_chessboard_index_(0),
      drop_mode_(previous_group.drop_mode_ == DropMode::GAMEWISE ? DropMode::GAMEWISE : DropMode::UNDEFINED), double_in_gamewise(0)
{
    new_chess = new Chess(previous_group.new_chess->id() + 1, false, current_player_);
    GroupGenerate(previous_group, takemode);
    Relax();
    board_gen = chessboard_group_.size();
}

ChessBoardGroup::~ChessBoardGroup() {
    chessboard_group_.clear();
    dropped_chess_group_.clear();
    delete new_chess;
}

void ChessBoardGroup::LPress(const coordinate &pos) {
    bool exists = false; // not gamewise
    int binary = (1 << chessboard_group_.size()) - 1;
    for (auto board : chessboard_group_)
        if (board->board_[pos.x()][pos.y()] != nullptr) exists = true;
    auto chess = chessboard_group_[activate_chessboard_index_]->board_[pos.x()][pos.y()];
    if (drop_mode_ == DropMode::CLASSIC) {
        // empty
        return;
    }
    if (drop_mode_ == DropMode::SUPERPO) {
        // do something
        if (!exists) {
            bool remove = false;
            for (auto dropped : dropped_chess_group_)
                if (((dropped.first >> activate_chessboard_index_) & 1) && dropped.second.pos_ == pos)
                {
                    remove = true;
                    reset();
                    break;
                }
            if (!remove) dropped_chess_group_.insert(std::make_pair(binary, ChessWithPos(new_chess, pos)));
        }
        return;
    }
    if (drop_mode_ == DropMode::ENTANGLE) {
        // do something
        if (exists) {
            auto backup = last_entangled_chess_.chess_;
            if (last_entangled_chess_.chess_ != nullptr)
                reset();
            if (chess != nullptr && chess->entangleable() && chess != backup) {
                drop_mode_ = DropMode::ENTANGLE;
                last_entangled_chess_ = {chess, pos};
            }
        } else {
            if (last_entangled_chess_.chess_ != nullptr) {
                int cnt = chessboard_group_.size();
                int tag = 0;
                auto p = last_entangled_chess_.pos_;
                for (int i = 0; i < cnt; ++i)
                    if (chessboard_group_[i]->board_[p.x()][p.y()] == last_entangled_chess_.chess_)
                        tag |= (1 << i);
                dropped_chess_group_.insert(std::make_pair(tag, ChessWithPos(new_chess, pos)));
                for (int i = 0; i < cnt; ++i)
                    if (!(tag & (1 << i))) {
                        auto size = board_size_;
                        for (int j = 0; j < size; ++j)
                            for (int k = 0; k < size; ++k)
                                if (chessboard_group_[i]->board_[j][k] == last_entangled_chess_.chess_)
                                    last_entangled_chess_.pos_ = {j, k};
                    }
            }
        }
        return;
    }
    if (drop_mode_ == DropMode::GAMEWISE) {
        // do something
        if (!chess) {
            bool remove = false;
            int dropped_cnt = 0;
            for (auto dropped : dropped_chess_group_)
                if (dropped.first == (1 << activate_chessboard_index_))
                    ++dropped_cnt, remove |= (pos == dropped.second.pos_);
            int tmp = (double_in_gamewise >> activate_chessboard_index_) & 1;
            qDebug() << "dropped_cnt" << dropped_cnt;
            qDebug() << "double_in_gamewise >> activate_chessboard_index_) & 1:" << tmp;
            if (remove) {
                if (dropped_cnt == 1 && ((double_in_gamewise >> activate_chessboard_index_) & 1))
                    --board_gen, double_in_gamewise &= ~(1 << activate_chessboard_index_);
                dropped_chess_group_.erase(std::make_pair(1 << activate_chessboard_index_, ChessWithPos(new_chess, pos)));
            }
            else if (dropped_cnt == ((double_in_gamewise >> activate_chessboard_index_) & 1))
                dropped_chess_group_.insert(std::make_pair(1 << activate_chessboard_index_, ChessWithPos(new_chess, pos)));
        }
        return;
    }
    if (drop_mode_ == DropMode::UNDEFINED) {
        // do something
        if (chess && chess->entangleable() && chess->player() != current_player_) {
            drop_mode_ = DropMode::ENTANGLE;
            last_entangled_chess_ = {chess, pos};
        } else if (!exists) {
            drop_mode_ = DropMode::CLASSIC;
            dropped_chess_group_.insert(std::make_pair(binary, ChessWithPos(new_chess, pos)));
        }
        return;
    }
}

void ChessBoardGroup::RPress(const coordinate &pos) {
    bool exists = false; // not gamewise
    int binary = (1 << chessboard_group_.size()) - 1;
    for (auto board : chessboard_group_)
        if (board->board_[pos.x()][pos.y()] != nullptr) exists = true;
    auto chess = chessboard_group_[activate_chessboard_index_]->board_[pos.x()][pos.y()];
    if (drop_mode_ == DropMode::CLASSIC) {
        // empty
        return;
    }
    if (drop_mode_ == DropMode::SUPERPO) {
        // empty
        return;
    }
    if (drop_mode_ == DropMode::ENTANGLE) {
        // do something
        if (!exists) {
            for (auto dropped : dropped_chess_group_)
                if (dropped.first & (1 << activate_chessboard_index_))
                    return;
            int cnt = chessboard_group_.size();
            int tag = 0, tot = 0;
            auto p = last_entangled_chess_.pos_;
            for (int i = 0; i < cnt; ++i)
                if (chessboard_group_[i]->board_[p.x()][p.y()] == last_entangled_chess_.chess_)
                    tag |= (1 << i), ++tot;
            if (board_gen + tot <= 8) {
                board_gen += tot;
                dropped_chess_group_.insert(std::make_pair(tag, ChessWithPos(new_chess, pos)));
            }
        }
        return;
    }
    if (drop_mode_ == DropMode::GAMEWISE) {
        // do something
        if (chess == nullptr) {
            for (auto dropped : dropped_chess_group_)
                if (dropped.first == (1 << activate_chessboard_index_))
                    return;
            if (board_gen < 8) {
                ++board_gen;
                double_in_gamewise |= (1 << activate_chessboard_index_);
                dropped_chess_group_.insert(std::make_pair(1 << activate_chessboard_index_, ChessWithPos(new_chess, pos)));
            }
        }
        return;
    }
    if (drop_mode_ == DropMode::UNDEFINED) {
        // do something
        if (!exists && board_gen <= 4) {
            drop_mode_ = DropMode::SUPERPO;
            board_gen *= 2;
            dropped_chess_group_.insert(std::make_pair(binary, ChessWithPos(new_chess, pos)));
        }
        return;
    }
}

void ChessBoardGroup::ScrollBoard(bool reverse) {
    if (!reverse)
        (activate_chessboard_index_ += 1) %= chessboard_group_.size();
    else
        (activate_chessboard_index_ += (chessboard_group_.size() - 1)) %= chessboard_group_.size();
}

void ChessBoardGroup::set_activate_chessboard_index(const int &activate_chessboard_index) {
    activate_chessboard_index_ = activate_chessboard_index;
}

void ChessBoardGroup::set_drop_mode(const DropMode &drop_mode) {drop_mode_ = drop_mode;}

void ChessBoardGroup::reset() {
    last_entangled_chess_ = nullptr;
    dropped_chess_group_.clear();
    board_gen = chessboard_group_.size();
    double_in_gamewise = 0;
    if (drop_mode_ != DropMode::GAMEWISE) drop_mode_ = DropMode::UNDEFINED;
}

/*
 * only called when construct the object
 */

void ChessBoardGroup::Relax() {
    const int &size = this->board_size_;
    std::vector<ChessBoard *> new_chessboard_group;
    for (auto board : chessboard_group_) {
        bool exists = false;
        for (auto new_board : new_chessboard_group) {
            bool same = true;
            for (int i = 0; i < size; ++i)
                for (int j = 0; j < size; ++j)
                    if (board->board_[i][j] != new_board->board_[i][j])
                        same = false;
            if (same) {
                exists = true;
                new_board->board_weight_ += board->board_weight_;
            }
        }
        if (!exists) new_chessboard_group.push_back(board);
    }
    swap(new_chessboard_group, chessboard_group_);
}

void ChessBoardGroup::GroupGenerate(const ChessBoardGroup &previous_group, const TakeMode &takemode) {
    // generate group without take any chess
    DropMode dropmode = previous_group.drop_mode_;
    if (dropmode == DropMode::SUPERPO)
        previous_group.new_chess->set_entangleable(true);
    const auto &group = previous_group.chessboard_group_;
    const auto &dropped = previous_group.dropped_chess_group_;
    int board_cnt = group.size();
    std::vector<int> weight_modifier(board_cnt);
    for (int i = 0; i < board_cnt; ++i)
        for (auto chess_pack : dropped)
            weight_modifier[i] += (chess_pack.first >> i) % 2;

    for (int i = 0; i < board_cnt; ++i)
        for (auto chess_pack : dropped)
            if ((chess_pack.first >> i) & 1) {
            auto new_board = new ChessBoard(*group[i]);
            new_board->board_weight_ = group[i]->board_weight_ * frac(1, weight_modifier[i]);
            auto chess_with_pos = chess_pack.second;
            auto pos = chess_with_pos.pos_;
            new_board->board_[pos.x()][pos.y()] = chess_with_pos.chess_;
            chessboard_group_.push_back(new_board);
        }
    // take chess
    if (takemode == TakeMode::QUANTUM) {
        const int &size = board_size_;
        int gate[10][size][size];
        memset(gate, 0, sizeof(gate));
        std::vector<coordinate> arr;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                arr.push_back({i, j});
        for (int i = 0; i < 10; ++i) {
            // find all gate
            for (auto board : chessboard_group_)
                for (auto pos : arr)
                    if (board->Captured(pos)) {
                        if (board->board_[pos.x()][pos.y()]->player() == Player::WHITE) gate[i][pos.x()][pos.y()] |= 1;
                        if (board->board_[pos.x()][pos.y()]->player() == Player::BLACK) gate[i][pos.x()][pos.y()] |= 2;
                    }
            bool change = false;
            for (auto pos : arr)
                change |= gate[i][pos.x()][pos.y()];
            if (!change)
                return;
            // find all quantum chess to be killed
            for (auto board : chessboard_group_)
                for (auto pos : arr)
                    if (gate[i][pos.x()][pos.y()] && board->board_[pos.x()][pos.y()] != nullptr
                            && board->board_[pos.x()][pos.y()]->entangleable())
                        board->board_[pos.x()][pos.y()]->set_entangleable(false);
            // gate effect
            for (auto pos : arr) {
                int tag = gate[i][pos.x()][pos.y()];
                if (tag == 0) continue;
                if (tag == 1) {
                    // white gate
                    Chess *first = nullptr;
                    for (auto board : chessboard_group_)
                        if (board->board_[pos.x()][pos.y()] != nullptr && board->board_[pos.x()][pos.y()]->player() == Player::WHITE)
                            if (first == nullptr || first->id() > board->board_[pos.x()][pos.y()]->id()) first = board->board_[pos.x()][pos.y()];
                    for (auto board : chessboard_group_) {
                        if (board->board_[pos.x()][pos.y()] == nullptr)
                            board->board_[pos.x()][pos.y()] = first;
                        else if (board->board_[pos.x()][pos.y()]->player() == Player::WHITE)
                            board->board_[pos.x()][pos.y()] = nullptr;
                    }
                }
                if (tag == 2) {
                    // black gate
                    Chess *first = nullptr;
                    for (auto board : chessboard_group_)
                        if (board->board_[pos.x()][pos.y()] != nullptr && board->board_[pos.x()][pos.y()]->player() == Player::BLACK)
                            if (first == nullptr || first->id() > board->board_[pos.x()][pos.y()]->id()) first = board->board_[pos.x()][pos.y()];
                    for (auto board : chessboard_group_) {
                        if (board->board_[pos.x()][pos.y()] == nullptr)
                            board->board_[pos.x()][pos.y()] = first;
                        else if (board->board_[pos.x()][pos.y()]->player() == Player::BLACK)
                            board->board_[pos.x()][pos.y()] = nullptr;
                    }
                }
                if (tag == 3) {
                    // destroy gate
                    for (auto board : chessboard_group_) board->board_[pos.x()][pos.y()] = nullptr;
                }
            }
            if (i == 9) {
                // destroy last 2 gate
                for (int j = 8; j < 10; ++j)
                    for (auto pos : arr)
                        if (gate[j][pos.x()][pos.y()])
                            for (auto board : chessboard_group_)
                                board->board_[pos.x()][pos.y()] = nullptr;
            }
        }
        return;
    }
    if (takemode == TakeMode::TAKEALL) {
        std::set<int> dead_chess_id;
        for (auto board : chessboard_group_) {
            const int &size = board_size_;
            for (int i = 0; i < size; ++i)
                for (int j = 0; j < size; ++j)
                    if (board->Captured(i, j) && board->board_[i][j]->player() == current_player_)
                        dead_chess_id.insert(board->board_[i][j]->id());
        }
        for (auto &board : chessboard_group_) {
            const int &size = board_size_;
            for (int i = 0; i < size; ++i)
                for (int j = 0; j < size; ++j)
                    if (board->board_[i][j] != nullptr)
                        if (dead_chess_id.find(board->board_[i][j]->id()) != dead_chess_id.end())
                            board->board_[i][j] = nullptr;
        }
        dead_chess_id.clear();
        for (auto board : chessboard_group_) {
            const int &size = board_size_;
            for (int i = 0; i < size; ++i)
                for (int j = 0; j < size; ++j)
                    if (board->Captured(i, j) && board->board_[i][j]->player() != current_player_)
                        dead_chess_id.insert(board->board_[i][j]->id());
        }
        for (auto &board : chessboard_group_) {
            const int &size = board_size_;
            for (int i = 0; i < size; ++i)
                for (int j = 0; j < size; ++j)
                    if (board->board_[i][j] != nullptr)
                        if (dead_chess_id.find(board->board_[i][j]->id()) != dead_chess_id.end())
                            board->board_[i][j] = nullptr;
        }
        return;
    }
}

