#pragma once

#include <vector>
#include <string>

// Direction of the game
enum Direction {
    NONE,
    LEFT,
    UP,
    RIGHT,
    DOWN
};

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

    /*
    Hash function that calculates the hash
    value for a point.

    @param p the point to calculate
    */
    static unsigned hash(const Point &p);

    /*
    Get the direction from this point to another.

    @param p the other point
    @return the direction from this point to point p.
            If the two points are not adjacent or are
            the same, return NONE.
    */
    Direction getDirectionTo(const Point &p) const;

    /*
    Get an adjacent point in a given direction.

    @param d the adjacent direction
    @return the adjacent point in the given direction
    */
    Point getOneAdjPoint(const Direction &d) const;

    /*
    Get all the adjacent points.
    Avoid using return value to return the result 
    in order to save cost.

    @param res the adjacent points will be stored in this field.
               Precondition: this field must be a vector of length 4.
    */
    void getAllAdjPoint(std::vector<Point> &res) const;

    /*
    @return the string that describe the content
            of the object.
    */
    std::string toString() const;

private:
    attr_type x;
    attr_type y;
};
