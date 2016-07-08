#include "Point.h"
#include "Convert.h"
#include "GameCtrl.h"

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

unsigned Point::hash(const Point &p) {
    std::hash<attr_type> h;
    return h(p.x) ^ h(p.y);
}

void Point::setAdjPoints(std::vector<Point> &res) const {
    int dx[] = {0, -1, 0, 1};
    int dy[] = {-1, 0, 1, 0};
    for (int i = 0; i < 4; ++i) {
        res[i].setX(x + dx[i]);
        res[i].setY(y + dy[i]);
    }
    for (unsigned i = 1; i < res.size(); ++i) {
        auto random = GameCtrl::getInstance()->random(0, i);
        Point tmp = res[i];
        res[i] = res[random];
        res[random] = tmp;
    }
}

Direction Point::getDirectionTo(const Point &p) const  {
    int dx = p.x - x;
    int dy = p.y - y;
    if (dx == 0 && dy == -1) {
        return LEFT;
    } else if (dx == 0 && dy == 1) {
        return RIGHT;
    } else if (dx == -1 && dy == 0) {
        return UP;
    } else if (dx == 1 && dy == 0) {
        return DOWN;
    } else {
        return NONE;
    }
}

std::string Point::toString() const {
    return "(" + Convert::toString(x) + ", " + Convert::toString(y) + ")";
}
