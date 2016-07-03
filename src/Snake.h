#pragma once

#include "Map.h"
#include <list>
#include <memory>

class Snake {
public:
    enum Direction {
        NONE,
        LEFT,
        UP,
        RIGHT,
        DOWN 
    };

    Snake();
    ~Snake();

    /*
    Move at current diection.
    */
    void move();

    /*
    Check whether the snake is dead.
    */
    bool isDead() const;

    /*
    Set move direction.
    */
    void setDirection(const Direction &d);

    /*
    Return move direction.
    */
    Direction getDirection() const;

    /*
    Set the map of the snake.
    */
    void setMap(std::shared_ptr<Map> m);

    /*
    Add a new body.

    @param p the point of the new body
    @return true if adding succeed, false otherwise
    */
    bool addBody(const Point &p);

private:
    bool dead = false;

    std::shared_ptr<Map> map;  // The map that the snake attaches to

    Direction direc = NONE;  // Current snake move direcition

    std::list<Point> body;

    /*
    Remove the tail of the snake.
    */
    void removeTail();

    /*
    Get a displacement of a direction.

    @param d the move direction
    @return the displacement stored in a Point object
    */
    static Point getDisplacement(const Direction &d);

    /*
    Return the head position.
    */
    const Point& getHead() const;

    /*
    Return the tail position.
    */
    const Point& getTail() const;
};
