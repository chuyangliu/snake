#pragma once

/*
A Point consists of x coordinate and 
y coordinate attributes.
*/
class Point {
public:
    typedef long AttrType;

    // Constants
    static const Point INVALID;

    // Constructor and destructor
    Point(const AttrType &x_ = 0, const AttrType &y_ = 0);
    ~Point();

    // Getters and setters
    AttrType getX() const;
    AttrType getY() const;
    void setX(const AttrType &x_);
    void setY(const AttrType &y_);

    // Operators
    friend Point operator+(const Point &a, const Point &b);
    friend bool operator==(const Point &a, const Point &b);
    friend bool operator!=(const Point &a, const Point &b);

private:
    AttrType x;
    AttrType y;
};
