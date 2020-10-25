#ifndef POINT_H
#define POINT_H
#include <QString>
#include <string>
#include <QDebug>
/*
 * point class:
 * 2d-point in chessboard or coordinate in window,
 * with natural operator
 */

class point
{
public:
    int x_, y_;
    point(const int &x, const int &y);
    int x() const;
    int y() const;
    void set_x(const int &x);
    void set_y(const int &y);
};

class frac : public point {
public:
    frac(const int &x, const int &y = 1);
    bool operator==(const frac &other) const;
    bool operator<(const frac &other) const;
    bool operator>(const frac &other) const;
    frac operator+(const frac &other) const;
    frac operator-(const frac &other) const;
    frac operator*(const frac &other) const;
    frac& operator+=(const frac &other);
    frac& Reduce();
    explicit operator QString() const;
};

class coordinate : public point {
public:
    coordinate(const int &x, const int &y);
    bool operator==(const coordinate &other) const;
    bool operator<(const coordinate &other) const;
    coordinate operator+(const coordinate &other) const;
    coordinate operator-(const coordinate &other) const;
    coordinate operator*(const int &n) const;
    coordinate operator%(const int &n) const;
    coordinate operator/(const int &n) const;
};

#endif // POINT_H
