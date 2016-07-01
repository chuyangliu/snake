#pragma once

#include "Grid.h"
#include "Point.h"

class Map {
public:
    Map(const unsigned &rowCnt_, const unsigned &colCnt_);
    ~Map();

    /*
    Forbid copy
    */
    Map(const Map &m) = delete;
    Map& operator=(const Map &m) = delete;

    /*
    Return the grid at the point.
    */
    Grid& at(const Point &p);
    const Grid& at(const Point &p) const;

    /*
    Check if the point hits the wall or the snake body of the map.

    @param p the point to check
    */
    bool hitBodyOrBoundary(const Point &p) const;

    /*
    Check if the point is inside the map.

    @param p the point to check
    */
    bool isInside(const Point &p) const;

    /*
    Check whether the map is filled with snake body.
    */
    bool isFilledWithBody() const;

    /*
    Create a food on the map randomly.
    */
    void createFood();

    /*
    Remove food if exists
    */
    void removeFood();

    /*
    Check if there is food on the map.
    */
    bool hasFood() const;

    /*
    Return the count of the rows.
    */
    unsigned getRowCount() const;

    /*
    Return the count of the columns.
    */
    unsigned getColCount() const;

    /*
    Return the food point.
    */
    const Point* getFoodPos() const;

private:
    unsigned rowCnt;
    unsigned colCnt;
    Point *foodPos = nullptr;
    Grid **content = nullptr;
};
