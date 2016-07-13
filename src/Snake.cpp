#include "Snake.h"
#include "GameCtrl.h"

using std::vector;
using std::list;
using std::shared_ptr;

Snake::Snake() {
}

Snake::~Snake() {
}

bool Snake::isDead() const {
    return dead;
}

bool Snake::addBody(const Point &p) {
    if (map && map->isInside(p)) {
        if (body.size() == 0) {  // Insert a head
            map->getGrid(p).setType(headType);
        } else {  // Insert a body
            if (body.size() > 1) {  // At least two body
                auto oldTail = getTail();
                map->getGrid(oldTail).setType(bodyType);
            }
            map->getGrid(p).setType(tailType);
        }
        body.push_back(p);
        return true;
    } else {
        return false;
    }
}

void Snake::setDirection(const Direction &d) {
    direc = d;
}

void Snake::setHeadType(const Grid::GridType &type) {
    headType = type;
}

void Snake::setBodyType(const Grid::GridType &type) {
    bodyType = type;
}

void Snake::setTailType(const Grid::GridType &type) {
    tailType = type;
}

Direction Snake::getDirection() const {
    return direc;
}

void Snake::setMap(std::shared_ptr<Map> m) {
    map = m;
}

const Point& Snake::getHead() const {
    return *body.begin();
}

const Point& Snake::getTail() const {
    return *body.rbegin();
}

Snake::size_type Snake::size() const {
    return body.size();
}

void Snake::removeTail() {
    if (map) {
        map->getGrid(getTail()).setType(Grid::GridType::EMPTY);
    }
    body.pop_back();
    if (body.size() > 1) {  // At least two body
        map->getGrid(getTail()).setType(tailType);
    }
}

void Snake::move() {
    if (isDead() || direc == NONE || !map) {
        return;
    }

    map->getGrid(getHead()).setType(bodyType);
    Point newHead = getHead().getOneAdjPoint(direc);
    body.push_front(newHead);

    if (map->isUnsafe(newHead)) {
        dead = true;
    }

    if (map->getGrid(newHead).getType() != Grid::GridType::FOOD) {
        removeTail();
    } else {
        map->removeFood();
    }

    map->getGrid(newHead).setType(headType);
}

void Snake::move(const std::list<Direction> &path) {
    for (const auto &d : path) {
        setDirection(d);
        move();
    }
}

void Snake::findPathTo(const int type, const Point &to, std::list<Direction> &path) {
    // To search the path to goal, first set the goal grid type to EMPTY
    // and then start searching because the original type of the goal
    // grid may be a FOOD or another type which is ignored by the search algorithm.
    // After searching, restore the goal grid type.
    auto originType = map->getGrid(to).getType();
    map->getGrid(to).setType(Grid::GridType::EMPTY);
    if (type == 0) {  // Search shortest path
        map->findMinPath(getHead(), to, path);
    } else if (type == 1) {  // Search longest path
        map->findMaxPath(getHead(), to, path);
    }
    map->getGrid(to).setType(originType);
}

void Snake::findMinPathToFood(std::list<Direction> &path) {
    findPathTo(0, map->getFood(), path);
}

void Snake::findMinPathToTail(std::list<Direction> &path) {
    findPathTo(0, getTail(), path);
}

void Snake::findMaxPathToTail(std::list<Direction> &path) {
    findPathTo(1, getTail(), path);
}

void Snake::decideNextDirection() {
    if (isDead() || !map || !map->hasFood()) {
        return;
    }

    // Copy a temp snake with a temp map
    // Create two search path (food and tail)
    Snake tmpSnake(*this);
    shared_ptr<Map> tmpMap = std::make_shared<Map>(*map);
    tmpSnake.setMap(tmpMap);
    list<Direction> pathToFood, pathToTail;

    // Step1:
    // If a path to food is found, move the temp snake to eat the food and to 
    // check if there is path to the tail of the temp snake. If there is no path
    // to tail after eating the food, it means that this path is dangerous and
    // this path will not be chosen.
    // Notice that only if the length of the path to tail is more than
    // 1 can the snake move to its tail because that the length equals
    // 1 means that the head is adjacent to the tail, which will make 
    // the snake die after moving towards the tail.
    tmpSnake.findMinPathToFood(pathToFood);
    if (!pathToFood.empty()) {
        tmpSnake.move(pathToFood);
        if (tmpMap->isFilledWithBody()) {  // Check if the map is full
            this->setDirection(*(pathToFood.begin()));
            return;
        } else {
            tmpSnake.findMaxPathToTail(pathToTail);
            if (pathToTail.size() > 1) {  // Check path to tail
                this->setDirection(*(pathToFood.begin()));
                return;
            }
        }
    }

    // Step2:
    // If no suitable path is found, make the snake move to its tail
    // along the longest way.
    this->findMaxPathToTail(pathToTail);
    if (pathToTail.size() > 1) {
        this->setDirection(*(pathToTail.begin()));
        return;
    }

    // Step3:
    // If no available path is found in step 1 and 2, then find a point
    // that is the farthest from the food.
    auto head = getHead();
    unsigned maxDist = 0;
    vector<Point> adjPoints(4, Point::INVALID);
    head.getAllAdjPoint(adjPoints);
    for (const auto &p : adjPoints) {
        if (!map->isUnsafe(p)) {
            unsigned d = Map::getManhattenDist(p, map->getFood());
            if (d > maxDist) {
                maxDist = d;
                direc = head.getDirectionTo(p);
            }
        }
    }
}
