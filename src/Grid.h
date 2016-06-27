#pragma once

class Grid {
public:
    /*
    Grid type declaration
    */
    enum GridType {
        EMPTY, WALL, FOOD, SNAKEHEAD, SNAKEBODY
    };

    Grid(GridType type_ = EMPTY);
    ~Grid();

    /*
    Getters and setters
    */
    GridType getType() const;

    void setType(GridType type_);

private:
    GridType type;
};
