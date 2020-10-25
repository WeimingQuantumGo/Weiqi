#include "chessboard.h"
#include <queue>

ChessBoard::ChessBoard(const int &board_size, const frac &board_weight)
    : board_size_(board_size), board_weight_(board_weight) {
    std::vector<Chess *> tmp;
    for (int i = 0; i < board_size_; ++i)
        tmp.push_back(nullptr);
    for (int i = 0; i < board_size_; ++i)
        board_.push_back(tmp);
}

ChessBoard::~ChessBoard() {board_.clear();}

/*
 * Using BFS
 */

bool ChessBoard::Captured(const int &x, const int &y) const {
    const int &size = board_size_;
    auto OutOfRange = [size](int u, int v) {return (u < 0 || u >= size || v < 0 || v >= size);};
    if (board_[x][y] == nullptr)
        return false;
    bool visit[board_size_][board_size_];
    memset(visit, 0, sizeof(visit));
    std::queue<coordinate> q;
    q.push(coordinate(x, y));
    visit[x][y] = true;
    coordinate dir[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    while (!q.empty()) {
        coordinate p = q.front();
        q.pop();
        for (auto delta : dir) {
            coordinate t = p + delta;
            if (OutOfRange(t.x(), t.y()) || visit[t.x()][t.y()]) continue;
            visit[t.x()][t.y()] = true;
            if (board_[t.x()][t.y()] == nullptr)
                return false;
            if (board_[t.x()][t.y()]->player() == board_[x][y]->player())
                q.push(t);
        }
    }
    return true;
}

bool ChessBoard::Captured(const coordinate &pos) const {return Captured(pos.x(), pos.y());}

bool ChessBoard::Possess(const Chess &chess) const {
    return false;
}
