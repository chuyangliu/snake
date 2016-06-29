#pragma once

#include "Map.h"
#include <list>
#include <mutex>

class Snake {
public:
    enum MoveDirection {
        NONE, LEFT, UP, RIGHT, DOWN 
    };

    Snake();
    ~Snake();

    /*
    Forbid copy
    */
    Snake(const Snake &m) = delete;
    Snake& operator=(const Snake &m) = delete;

    /*
    Return the moveArea field.
    */
    Map* getMoveArea();
    const Map* getMoveArea() const;

    /*
    Move at current diection.
    This is a thread-safe method.
    */
    void move();

    /*
    Check whether the snake is dead.
    */
    bool isDead() const;

    /*
    Set move direction.
    */
    void setMoveDirection(const MoveDirection &d);

    /*
    Return move direction.
    */
    MoveDirection getMoveDirection() const;

private:
    Map *moveArea = nullptr;
    MoveDirection direc = NONE;
    bool dead = false;
    std::list<Point> body;

    std::mutex mutex;

    /*
    Add new body.

    @param p the point of the new body
    @return true if adding succeed, false otherwise
    */
    bool addBody(const Point &p);

    /*
    Remove the tail of the snake.
    */
    void removeTail();

    /*
    Get the displacement of the direction.
    */
    static Point getDisplacement(const MoveDirection &d);

    /*
    Return the head position.
    */
    Point getHeadPos() const;

    /*
    Return the tail position.
    */
    Point getTailPos() const;
};
