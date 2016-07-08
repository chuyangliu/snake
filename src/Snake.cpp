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

Point Snake::getDisplacement(const Direction &d) {
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

    map->getGrid(getHead()).setType(bodyType);
    Point newHead = getHead() + getDisplacement(direc);
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

void Snake::findMinPathToFood(std::list<Direction> &path) {
    // To search the path to food, first set the food grid type to EMPTY
    // and then start searching because the original type of the food
    // grid is a FOOD which is ignored by the search algorithm.
    // After searching, restore the grid type of the food.
    auto food = map->getFood();
    map->getGrid(food).setType(Grid::GridType::EMPTY);
    map->findMinPath(getHead(), food, path);
    map->getGrid(food).setType(Grid::GridType::FOOD);
}

void Snake::findMinPathToTail(std::list<Direction> &path) {
    // To search the path to tail, first set the tail's type to EMPTY
    // and then start searching because the original type of the tail
    // grid is a SNAKEBODY which is ignored by the search algorithm.
    // After searching, restore the grid type of the tail.
    auto tail = getTail();
    auto originType = map->getGrid(tail).getType();
    map->getGrid(tail).setType(Grid::GridType::EMPTY);
    map->findMinPath(getHead(), tail, path);
    map->getGrid(tail).setType(originType);
}

void Snake::decideNextDirection() {
    if (isDead() || !map) {
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
        tmpSnake.findMinPathToTail(pathToTail);
        if (pathToTail.size() > 1) {
            this->setDirection(*(pathToFood.begin()));
            return;
        }
    }

    // Step2:
    // If no suitable path is found, make the snake move to its tail.
    this->findMinPathToTail(pathToTail);
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
    head.setAdjPoints(adjPoints);
    for (const auto &p : adjPoints) {
        if (!map->isUnsafe(p)) {
            unsigned d = Map::getManhattenDistance(p, map->getFood());
            if (d > maxDist) {
                maxDist = d;
                direc = head.getDirectionTo(p);
            }
        }
    }
}
