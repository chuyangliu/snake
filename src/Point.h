#pragma once

class Point {
public:
    int x;
    int y;

    Point(const int &x_ = 0, const int &y_ = 0);
    ~Point();

    friend Point operator+(const Point &a, const Point &b);
};