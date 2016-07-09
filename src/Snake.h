#pragma once

#include "Map.h"
#include <memory>

class Snake {
public:
    typedef Map::size_type size_type;

    Snake();
    ~Snake();

    /*
    Move at current direction.
    */
    void move();

    /*
    Move steps specified by a list of direcition.

    @param path the move path
    */
    void move(const std::list<Direction> &path);

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

    /*
    Set the head type of the snake.
    */
    void setHeadType(const Grid::GridType &type);

    /*
    Set the body type of the snake.
    */
    void setBodyType(const Grid::GridType &type);

    /*
    Set the tail type of the snake.
    */
    void setTailType(const Grid::GridType &type);

    /*
    Return the snake bodies size.
    */
    size_type size() const;

    /*
    Start snake AI to decide the next move direction.
    After calling this method, the direc field will be set
    to the next move direction.
    */
    void decideNextDirection();

private:
    bool dead = false;

    std::shared_ptr<Map> map;  // The map that the snake attaches to

    Direction direc = NONE;  // Current snake move direcition

    std::list<Point> body;

    Grid::GridType headType;
    Grid::GridType bodyType;
    Grid::GridType tailType;

    /*
    Find the shortest path from snake's head to the food.

    @param path the result will be stored in this field.
                If there is no available path, the length
                of this field will be zero.
    */
    void findMinPathToFood(std::list<Direction> &path);

    /*
    Find the shortest path from snake's head to the tail.

    @param path the result will be stored in this field.
                If there is no available path, the length
                of this field will be zero.
    */
    void findMinPathToTail(std::list<Direction> &path);

    /*
    Find the longest(approximately) path from snake's head to the tail.

    @param path the result will be stored in this field
                If there is no available path, the length
                of this field will be zero.
    */
    void findMaxPathToTail(std::list<Direction> &path);

    /*
    Find a path from the snake's head to the goal point.

    @param type if type is 0, find the shortest path,
                else if type is 1, find the longest path.
    @param to the goal point
    @param path the result will be stored in this field
    */
    void findPathTo(const int type, const Point &to, std::list<Direction> &path);

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
