#pragma once

#include "Map.h"
#include <list>

class Snake {
public:
    enum MoveDirection {
        NONE,
        LEFT,
        UP,
        RIGHT,
        DOWN 
    };

    Snake() = delete;
    Snake(const unsigned &mapRowCnt, const unsigned &mapColCnt);
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
    bool dead = false;

    Map *moveArea = nullptr;
    MoveDirection direc = NONE;

    std::list<Point> body;

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
