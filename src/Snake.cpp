#include "Snake.h"
#include "GameCtrl.h"
#include <new>

Snake::Snake(const unsigned &mapRowCnt, const unsigned &mapColCnt) {
    moveArea = new(std::nothrow) Map(mapRowCnt, mapColCnt);
    if (!moveArea) {
        GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
    }
    // Create three bodies first
    addBody(Point(1, 3));
    addBody(Point(1, 2));
    addBody(Point(1, 1));
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

bool Snake::isDead() const {
    return dead;
}

bool Snake::addBody(const Point &p) {
    if (moveArea->isInside(p)) {
        body.push_back(p);
        if (body.size() == 1) {  // Insert a head
            moveArea->getGrid(p).setType(Grid::GridType::SNAKEHEAD);
        } else {
            moveArea->getGrid(p).setType(Grid::GridType::SNAKEBODY);
        }
        return true;
    } else {
        return false;
    }
}

void Snake::setMoveDirection(const MoveDirection &d) {
    direc = d;
}

Snake::MoveDirection Snake::getMoveDirection() const {
    return direc;
}

void Snake::move() {
    if (isDead() || direc == NONE) {
        return;
    }

    moveArea->getGrid(getHeadPos()).setType(Grid::GridType::SNAKEBODY);
    Point newHead = getHeadPos() + getDisplacement(direc);
    body.push_front(newHead);

    if (moveArea->getGrid(newHead).getType() != Grid::GridType::FOOD) {
        removeTail();
    } else {
        moveArea->removeFood();
    }

    if (moveArea->isBodyOrBoundary(newHead)) {
        dead = true;
    }

    moveArea->getGrid(newHead).setType(Grid::GridType::SNAKEHEAD);
}

Point Snake::getHeadPos() const {
    return *body.begin();
}

Point Snake::getTailPos() const {
    return *body.rbegin();
}

void Snake::removeTail() {
    moveArea->getGrid(getTailPos()).setType(Grid::GridType::EMPTY);
    body.pop_back();
}

Point Snake::getDisplacement(const MoveDirection &d) {
    int dx = 0, dy = 0;
    switch (d) {
        case LEFT:
            dx = 0; dy = -1; break;
        case UP:
            dx = -1; dy = 0; break;
        case RIGHT:
            dx = 0; dy = 1; break;
        case DOWN:
            dx = 1; dy = 0; break;
        default:
            return Point(0, 0);
    }
    return Point(dx, dy);
}
