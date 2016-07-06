#pragma once

#include "Grid.h"
#include <vector>
#include <list>

/*
A map of the game
*/
class Map {
public:
    typedef std::vector<std::vector<Grid>> content_type;
    typedef content_type::size_type size_type;

    Map(const size_type &rowCnt_ = 20, const size_type &colCnt_ = 20);
    ~Map();

    /*
    Return the grid at the point.
    Two versions for const and non-const object.
    */
    Grid& getGrid(const Point &p);
    const Grid& getGrid(const Point &p) const;

    /*
    Check if the grid at the point is a snake body/head
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
    Return the food position.
    */
    const Point& getFood() const;

    /*
    Return the count of the rows.
    */
    size_type getRowCount() const;

    /*
    Return the count of the columns.
    */
    size_type getColCount() const;

    /*
    Find the shortest path from the start
    point to the end point.

    @param from the start point
    @param to the end point
    @param path the result will be stored as a list of Direction
                in this field
    */
    void findMinPath(const Point &from, const Point &to, std::list<Direction> &path);

private:
    Point food = Point::INVALID;

    content_type content;  // The elements on the map

    /*
    Set default walls on the map.
    */
    void setDefaultWalls();

    /*
    Get a food position on the map
    WARNING: if there is no space for food,
             this will be an endless loop.

    @return the new food position
    */
    Point getFoodPos() const;
};
