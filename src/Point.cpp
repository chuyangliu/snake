#include "Point.h"

const Point Point::INVALID = Point(-1, -1);

Point::Point(const attr_type &x_, const attr_type &y_)
    : x(x_), y(y_) {
}

Point::~Point() {
}


Point::attr_type Point::getX() const {
    return x;
}

Point::attr_type Point::getY() const {
    return y;
}

void Point::setX(const attr_type &x_) {
    x = x_;
}

void Point::setY(const attr_type &y_) {
    y = y_;
}

Point operator+(const Point &a, const Point &b) {
    return Point(a.x + b.x, a.y + b.y);
}

bool operator==(const Point &a, const Point &b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point &a, const Point &b) {
    return !(a == b);
}
