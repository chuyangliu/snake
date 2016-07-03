#pragma once

#include "Point.h"

/*
A Grid on the game map.
*/
class Grid {
public:
    enum GridType {
        EMPTY,
        WALL,
        FOOD,
        SNAKEBODY1,
        SNAKEBODY2,
        SNAKEHEAD1,
        SNAKEHEAD2
    };

    // Constructor and destructor
    Grid(GridType type_ = EMPTY);
    ~Grid();

    // Getters and setters
    GridType getType() const;
    void setType(GridType type_);

private:
    GridType type;
    //Point p;
};
