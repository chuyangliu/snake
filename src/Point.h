#pragma once

/*
A Point consists of x coordinate and 
y coordinate attributes.
*/
class Point {
public:
    typedef long attr_type;

    // Constants
    static const Point INVALID;

    // Constructor and destructor
    Point(const attr_type &x_ = 0, const attr_type &y_ = 0);
    ~Point();

    // Getters and setters
    attr_type getX() const;
    attr_type getY() const;
    void setX(const attr_type &x_);
    void setY(const attr_type &y_);

    // Operators
    friend Point operator+(const Point &a, const Point &b);
    friend bool operator==(const Point &a, const Point &b);
    friend bool operator!=(const Point &a, const Point &b);

private:
    attr_type x;
    attr_type y;
};
