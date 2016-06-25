#pragma once

class Grid {
public:
    // Grid type declaration
    enum GridType {
        BLANK, WALL
    };

    // Constructors
    Grid();
    Grid(GridType type_);

    // Getters and setters
    GridType getType() const;
    void setType(GridType type_);

private:
    GridType type;
};
