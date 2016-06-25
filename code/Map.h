#pragma once

#include "Grid.h"

class Map {
public:
    Map(const unsigned &rowNum_, const unsigned &colNum_);
    ~Map();

    // Forbid copy
    Map(const Map &m) = delete;
    Map& operator=(const Map &m) = delete;

    /*
    Return the grid at row x and column y.
    */
    Grid& at(const unsigned &x, const unsigned &y);
    const Grid& at(const unsigned &x, const unsigned &y) const;

    // Getters and setters
    unsigned getRowCount() const;
    unsigned getColCount() const;

private:
    unsigned rowCnt;
    unsigned colCnt;

    Grid **content = nullptr;
};
