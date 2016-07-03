#include "Point.h"

const Point Point::INVALID = Point(-1, -1);

Point::Point(const AttrType &x_, const AttrType &y_)
    : x(x_), y(y_) {
}

Point::~Point() {
}


Point::AttrType Point::getX() const {
    return x;
}

Point::AttrType Point::getY() const {
    return y;
}

void Point::setX(const AttrType &x_) {
    x = x_;
}

void Point::setY(const AttrType &y_) {
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
