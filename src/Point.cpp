#include "Point.h"

Point::Point(const int &x_, const int &y_) : x(x_), y(y_) {
}

Point::~Point() {
}

Point operator+(const Point &a, const Point &b) {
    return Point(a.x + b.x, a.y + b.y);
}
