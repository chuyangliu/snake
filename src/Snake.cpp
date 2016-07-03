#include "Snake.h"
#include "GameCtrl.h"

Snake::Snake(Map *m) : map(m) {
}

Snake::~Snake() {
}

bool Snake::isDead() const {
    return dead;
}

bool Snake::addBody(const Point &p) {
    if (map && map->isInside(p)) {
        if (body.size() == 0) {  // Insert a head
            map->getGrid(p).setType(Grid::GridType::SNAKEHEAD);
        } else {  // Insert a body
            map->getGrid(p).setType(Grid::GridType::SNAKEBODY);
        }
        body.push_back(p);
        return true;
    } else {
        return false;
    }
}

void Snake::setDirection(const MoveDirection &d) {
    direc = d;
}

Snake::MoveDirection Snake::getDirection() const {
    return direc;
}

void Snake::setMap(Map *m) {
    map = m;
}

const Point& Snake::getHead() const {
    return *body.begin();
}

const Point& Snake::getTail() const {
    return *body.rbegin();
}

void Snake::removeTail() {
    if (map) {
        map->getGrid(getTail()).setType(Grid::GridType::EMPTY);
    }
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

void Snake::move() {
    if (isDead() || direc == NONE || !map) {
        return;
    }

    map->getGrid(getHead()).setType(Grid::GridType::SNAKEBODY);
    Point newHead = getHead() + getDisplacement(direc);
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
