#include "Snake.h"
#include "GameCtrl.h"
#include <new>

Snake::Snake() {
    moveArea = new(std::nothrow) Map(GameCtrl::MAP_ROW, GameCtrl::MAP_COL);
    if (!moveArea) {
        GameCtrl::getInstance()->exitWithException("Not enough memory.\n");
    }
}

Snake::~Snake() {
    delete moveArea;
}

Map* Snake::getMoveArea() {
    return moveArea;
}

const Map* Snake::getMoveArea() const {
    return moveArea;
}
