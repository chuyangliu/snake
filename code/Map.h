#pragma once

#include "Grid.h"

class Map {
public:
    Map();
    ~Map();

    // Forbid copy
    Map(const Map &m) = delete;
    Map& operator=(const Map &m) = delete;

private:
    Grid **content = nullptr;
};
