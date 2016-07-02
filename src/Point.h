#pragma once

/*
A Point consists of x coordinate and 
y coordinate attributes.
*/
class Point {
public:
    typedef long AttrType;

    Point(const AttrType &x_ = 0, const AttrType &y_ = 0);
    ~Point();

    /*
    Getters and setters
    */
    AttrType getX() const;
    AttrType getY() const;
    void setX(const AttrType &x_);
    void setY(const AttrType &y_);

    /*
    Operator overload.
    */
    friend Point operator+(const Point &a, const Point &b);

private:
    AttrType x;
    AttrType y;
};
