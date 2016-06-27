#pragma once

class Grid {
public:
    /*
    Grid type declaration
    */
    enum GridType {
        BLANK, WALL, FOOD
    };

    Grid(GridType type_ = BLANK);
    ~Grid();

    /*
    Getters and setters
    */
    GridType getType() const;

    void setType(GridType type_);

private:
    GridType type;
};
