#include "point.h"

point::point(const int &x, const int &y) : x_(x), y_(y) {}
int point::x() const { return x_; }
int point::y() const { return y_; }
void point::set_x(const int &x) { x_ = x; }
void point::set_y(const int &y) { y_ = y; }

frac::frac(const int &x, const int &y) : point(x, y) {}
bool frac::operator==(const frac &other) const { return x_ * other.y_ == y_ * other.x_; }
bool frac::operator<(const frac &other) const { return x_ * other.y_ < y_ * other.x_; }
bool frac::operator>(const frac &other) const { return x_ * other.y_ > y_ * other.x_; }
frac frac::operator+(const frac &other) const { return frac(x_ * other.y_ + y_ * other.x_, y_ * other.y_); }
frac frac::operator-(const frac &other) const { return frac(x_ * other.y_ - y_ * other.x_, y_ * other.y_); }
frac frac::operator*(const frac &other) const { return frac(x_ * other.x_, y_ * other.y_); }
frac& frac::operator+=(const frac &other) {
    (x_ *= other.y_) += y_ * other.x_;
    y_ *= other.y_;
    return *this;
}
frac& frac::Reduce() {
    std::function<int(int, int)> gcd = [&](int x, int y) { return y ? gcd(y, x % y) : x; };
    int t = gcd(x_, y_);
    x_ /= t, y_ /= t;
    return *this;
}
frac::operator QString() const { return QString(std::to_string(x_).data()) + '/' + std::to_string(y_).data(); }

coordinate::coordinate(const int &x, const int &y) : point(x, y) {}
bool coordinate::operator==(const coordinate &other) const { return x_ == other.x_ && y_ == other.y_; }
bool coordinate::operator<(const coordinate &other) const {return x_ < other.x_ || (x_ == other.x_ && y_ < other.y_);}
coordinate coordinate::operator+(const coordinate &other) const { return coordinate(x_ + other.x_, y_ + other.y_); }
coordinate coordinate::operator-(const coordinate &other) const { return coordinate(x_ - other.x_, y_ - other.y_); }
coordinate coordinate::operator*(const int &n) const { return coordinate(x_ * n, y_ * n); }
coordinate coordinate::operator%(const int &n) const { return coordinate(x_ % n, y_ % n); }
coordinate coordinate::operator/(const int &n) const { return coordinate(x_ / n, y_ / n); }
