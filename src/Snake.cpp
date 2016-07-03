#include "Snake.h"
#include "GameCtrl.h"

Snake::Snake(Map *m) : map(m) {
    // Create three bodies initially
    addBody(Point(1, 3));
    addBody(Point(1, 2));
    addBody(Point(1, 1));
}

Snake::~Snake() {
}

bool Snake::isDead() const {
    return dead;
}

bool Snake::addBody(const Point &p) {
    if (map->isInside(p)) {
        body.push_back(p);
        if (body.size() == 1) {  // Insert a head
            map->getGrid(p).setType(Grid::GridType::SNAKEHEAD);
        } else {
            map->getGrid(p).setType(Grid::GridType::SNAKEBODY);
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

    map->getGrid(getHeadPos()).setType(Grid::GridType::SNAKEBODY);
    Point newHead = getHeadPos() + getDisplacement(direc);
    body.push_front(newHead);

    if (map->getGrid(newHead).getType() != Grid::GridType::FOOD) {
        removeTail();
    } else {
        map->removeFood();
    }

    if (map->isBodyOrBoundary(newHead)) {
        dead = true;
    }

    map->getGrid(newHead).setType(Grid::GridType::SNAKEHEAD);
}

const Point& Snake::getHeadPos() const {
    return *body.begin();
}

const Point& Snake::getTailPos() const {
    return *body.rbegin();
}

void Snake::removeTail() {
    map->getGrid(getTailPos()).setType(Grid::GridType::EMPTY);
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
