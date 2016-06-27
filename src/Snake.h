#pragma once

#include "Map.h"

class Snake {
public:
    Snake();
    ~Snake();

    /*
    Forbid copy
    */
    Snake(const Snake &m) = delete;
    Snake& operator=(const Snake &m) = delete;

    /*
    Return the moveArea field.
    */
    Map* getMoveArea();
    const Map* getMoveArea() const;

private:
    Map *moveArea = nullptr;
};
