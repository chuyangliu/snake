#pragma once

#include "Grid.h"
#include "Point.h"

/*
A subclass of Grid that
contains fields for searching algorithms.
*/
class SearchableGrid : public Grid {
public:
    typedef int value_type;

    // Constructor and destructor
    SearchableGrid();
    virtual ~SearchableGrid();

    // Getters and setters
    void setG(const value_type g_);
    void setH(const value_type h_);
    void setParent(const Direction &p_);
    value_type getG() const;
    value_type getH() const;
    value_type getF() const;  // f = g + h
    Direction getParent() const;

private:
    // the cost from the start grid to this grid
    value_type g = 0;

    // the cost from this grid to the goal grid (Heuristic value)
    value_type h = 0;

    // The parent direction
    Direction parent = NONE;
};
