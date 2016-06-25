#pragma once

class Grid {
public:
    // Grid type declaration
    enum GridType {
        BLANK, WALL
    };

    Grid();
    Grid(GridType type_);
    ~Grid();

    // Getters and setters
    GridType getType() const;
    void setType(GridType type_);

private:
    GridType type;
};
