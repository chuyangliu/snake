#pragma once

#include "Grid.h"
#include "Point.h"

/*
A map of the game
*/
class Map {
public:
    Map(const long &rowCnt_, const long &colCnt_);
    ~Map();

    /*
    Forbid copy(temporary)
    */
    Map(const Map &m) = delete;
    Map& operator=(const Map &m) = delete;

    /*
    Return the grid at the point.
    */
    Grid& at(const Point &p);
    const Grid& at(const Point &p) const;

    /*
    Check if the grid at the point is a snake body
    or map boundary.

    @param p the point to check
    */
    bool isBodyOrBoundary(const Point &p) const;

    /*
    Check if the point is inside the map
    (not include the boundary).

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
    long getRowCount() const;

    /*
    Return the count of the columns.
    */
    long getColCount() const;

    /*
    Return the food position.
    */
    const Point* getFoodPos() const;

private:
    long rowCnt;
    long colCnt;
    Point *foodPos = nullptr;
    Grid **content = nullptr;
};
