#include "Snake.h"
#include "GameCtrl.h"
#include <new>

Snake::Snake() {
    moveArea = new(std::nothrow) Map(GameCtrl::MAP_ROW, GameCtrl::MAP_COL);
    if (!moveArea) {
        GameCtrl::getInstance()->exitWithException("Not enough memory.\n");
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

bool Snake::addBody(const Point &p) {
    if (moveArea->isInside(p)) {
        body.push_back(p);
        if (body.size() == 1) {  // Insert a head
            moveArea->at(p.x, p.y).setType(Grid::GridType::SNAKEHEAD);
        } else {
            moveArea->at(p.x, p.y).setType(Grid::GridType::SNAKEBODY);
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

void Snake::move(const bool &removeOldTail) {
    if (direc == NONE || moveArea->hitBoundary(getHeadPos())) return;

    if (removeOldTail) {
        Point tail = getTailPos();
        moveArea->at(tail.x, tail.y).setType(Grid::GridType::EMPTY);
        body.erase(--body.end());  // Remove old tail
    }

    // Calculate direction displacement
    int dx = 0, dy = 0;
    switch (direc) {
        case LEFT:
            dx = 0;
            dy = -1;
            break;
        case UP:
            dx = -1;
            dy = 0;
            break;
        case RIGHT:
            dx = 0;
            dy = 1;
            break;
        case DOWN:
            dx = 1;
            dy = 0;
            break;
        default:
            return;
    }

    // Create new head
    Point head = getHeadPos();
    moveArea->at(head.x, head.y).setType(Grid::GridType::SNAKEBODY);
    Point newHead = Point(head.x + dx, head.y + dy);
    body.push_front(newHead);
    moveArea->at(newHead.x, newHead.y).setType(Grid::GridType::SNAKEHEAD);
}

Point Snake::getHeadPos() const {
    return *body.begin();
}

Point Snake::getTailPos() const {
    return *body.rbegin();
}
