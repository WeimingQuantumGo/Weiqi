#include "chess.h"

Chess::Chess(const int &id, const bool &entangleable, const Player &player)
    : id_(id), player_(player), entangleable_(entangleable) {}

int Chess::id() const {return id_;}
Player Chess::player() const {return player_;}
bool Chess::entangleable() const {return entangleable_;}
void Chess::set_entangleable(const bool &entangleable) {entangleable_ = entangleable;}
Chess::operator QString() const {return std::to_string(id_).data();}

ChessWithPos::ChessWithPos(Chess *chess, const coordinate &pos) : chess_(chess), pos_(pos) {}
bool ChessWithPos::operator<(const ChessWithPos &other) const {return pos_ < other.pos_;}
