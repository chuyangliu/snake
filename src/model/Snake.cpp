#include "model/Snake.h"
#include "util/util.h"
#include "GameCtrl.h"
#include <queue>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::list;
using std::queue;
using util::Random;

Snake::Snake() {}

Snake::~Snake() {}

void Snake::setDirection(const Direction &d) {
    direc = d;
}

void Snake::setMap(Map *const m) {
    map = m;
}

// This function returns the first direction in current snake's searched path 
Direction Snake::getDirection() const {
    return direc;
}

//Function to check if Snake is dead or alive
bool Snake::isDead() const {
    return dead;
}

//Test function finding the minist path
void Snake::testMinPath(const Pos &from, const Pos &to, std::list<Direction> &path) {
    map->setTestEnabled(true);
    findMinPath(from, to, path);
    map->showPath(from, path);
    map->setTestEnabled(false);
}

//Test function finding the longest path
void Snake::testMaxPath(const Pos &from, const Pos &to, std::list<Direction> &path) {
    map->setTestEnabled(true); //The test enablement check variable in map is true. Start of function
    findMaxPath(from, to, path); //Initialize path variables according to the start and end values ​​and directions in the function
    Pos cur = from; //The current cursor position is initialized to the beginning value
    for (const Direction d : path) {
        map->getPoint(cur).setDist(GameCtrl::EMPTY_VALUE);
        cur = cur.getAdj(d); //The position of the current cursor returns the position adjacent in the specified direction
    }
    map->getPoint(from).setDist(0);
    map->getPoint(to).setDist(1);
    map->showPath(from, path);
    map->setTestEnabled(false); //The test validation check variable in map is false.Terminating the function
}

void Snake::testHamilton() {
    map->setTestEnabled(true);
    enableHamilton();
    SizeType row = map->getRowCount();
	SizeType col = map->getColCount();
    for (SizeType i = 1; i < row - 1; ++i) {
        for (SizeType j = 1; j < col - 1; ++j) {
            Pos pos = Pos(i, j);
            Point &point = map->getPoint(pos);
            point.setDist(point.getIndex());
            map->showPos(pos);
        }
    }
}

// This function is for add snake's head / body when only initialize phase 
void Snake::addBody(const Pos &p) {
    if (bodies.size() == 0) {  // Insert a head
        map->getPoint(p).setType(Point::Type::SNAKE_HEAD);
    } else {  // Insert a body
        if (bodies.size() > 1) {
            const Pos &oldTail = getTail();
            map->getPoint(oldTail).setType(Point::Type::SNAKE_BODY);
        }
        map->getPoint(p).setType(Point::Type::SNAKE_TAIL);
    }
    bodies.push_back(p);
}

// this function moves snake's position toward searched path direction
// removeTail() when snake don't eat Food
// when snake eat food, removeTail is not called, so snake's length is increased one
void Snake::move() {
    if (isDead() || direc == NONE) {
        return;
    }
    map->getPoint(getHead()).setType(Point::Type::SNAKE_BODY);
    Pos newHead = getHead().getAdj(direc);
    bodies.push_front(newHead);
    if (!map->isSafe(newHead)) {
        dead = true;
    } else {
        if (map->getPoint(newHead).getType() != Point::Type::FOOD) {
            removeTail();
        } else {
            map->removeFood();
        }
    }
    map->getPoint(newHead).setType(Point::Type::SNAKE_HEAD);
}

// this function is for only virtual snake
// this function moves all directions in the list(searched path) for simulation the path is exist
void Snake::move(const std::list<Direction> &path) {
    for (const Direction &d : path) {
        setDirection(d);
        move();
    }
}

void Snake::enableHamilton() {
	// Hamilton Cycle only when count of row, col is even.
    if (map->getRowCount() % 2 == 1 && map->getColCount() % 2 == 1) {
        throw std::range_error("Snake.enableHamilton(): require even amount of rows or columns.");
    }
    hamiltonEnabled = true;
    buildHamilton();
}

// This function is for deciding next snake's path when enableAI = true
// This function's main goal is add next direction to snake instance
// graph based search :
// step 1 : search min path from current virtual snake position to Food
// step 2 : move virtual snake to searched path and when the virtual snake' body is full in the map, set the first searched direction to real snake  
// step 3 : not step2(when the virtual snake' body is full in the map), search max path from virtual snake's head to virtual snake's Tail
//			if path is exist, return the first direction to real snake
// step 4 : not step 3(path is exist), search max path from real snake's head to real snake's tail
// step 5 : not step 4(path is exist), search the max path from current to Food position and set direction the first path
// * Caution!! step 3's virtual snake and step 4's real snake's position is different, because when step 2, virtual snake is moved.
void Snake::decideNext() {
    if (isDead()) {
void Snake::decideNext() {
    if (isDead()) {
        return;

    } else if (!isDead() && !map->hasFood()) {
        direc = NONE;
        return;
    }

    if (hamiltonEnabled) {  // AI based on the Hamiltonian cycle

        SizeType size = map->getSize();
        Pos head = getHead();
		Pos tail = getTail();
        Point::ValueType tailIndex = map->getPoint(tail).getIndex();
        Point::ValueType headIndex = map->getPoint(head).getIndex();
        // Try to take shortcuts when the snake is not long enough
        if (bodies.size() < size * 3 / 4) {
            list<Direction> minPath;
            findMinPathToFood(minPath);
            if (!minPath.empty()) {
                Direction nextDirec = *minPath.begin();
                Pos nextPos = head.getAdj(nextDirec);
                Point::ValueType nextIndex = map->getPoint(nextPos).getIndex();
                Point::ValueType foodIndex = map->getPoint(map->getFood()).getIndex();
                headIndex = util::getDistance(tailIndex, headIndex, (Point::ValueType)size);
                nextIndex = util::getDistance(tailIndex, nextIndex, (Point::ValueType)size);
                foodIndex = util::getDistance(tailIndex, foodIndex, (Point::ValueType)size);
                if (nextIndex > headIndex && nextIndex <= foodIndex) {
                    direc = nextDirec;
                    return;
                }
            }
        }
        // Move along the hamitonian cycle
        headIndex = map->getPoint(head).getIndex();
        vector<Pos> adjPositions = head.getAllAdj();
        for (const Pos &adjPos : adjPositions) {
            const Point &adjPoint = map->getPoint(adjPos);
            Point::ValueType adjIndex = adjPoint.getIndex();
            if (adjIndex == (headIndex + 1) % size) {
                direc = head.getDirectionTo(adjPos);
            }
        }

    } else {  // AI based on graph search

        list<Direction> pathToFood; 
		list<Direction> pathToTail;
        // Create a virtual snake
        Map tmpMap = *map;
        Snake tmpSnake(*this);
        tmpSnake.setMap(&tmpMap);
        // Step 1
        tmpSnake.findMinPathToFood(pathToFood);
        if (!pathToFood.empty()) {
            // Step 2
            tmpSnake.move(pathToFood);
            if (tmpMap.isAllBody()) {
                this->setDirection(*(pathToFood.begin()));
                return;
            } else {
                // Step 3
                tmpSnake.findMaxPathToTail(pathToTail);
                if (pathToTail.size() > 1) {
                    this->setDirection(*(pathToFood.begin()));
                    return;
                }
            }
        }
        // Step 4
        this->findMaxPathToTail(pathToTail);
        if (pathToTail.size() > 1) {
            this->setDirection(*(pathToTail.begin()));
            return;
        }
        // Step 5
        direc = Direction::DOWN;  // A default direction
        SizeType max = 0;
        Pos head = getHead();
        vector<Pos> adjPositions = head.getAllAdj();
        for (const Pos &adjPos : adjPositions) {
            if (map->isSafe(adjPos)) {
                SizeType dist = Map::distance(adjPos, map->getFood());
                if (dist >= max) {
                    max = dist;
                    direc = head.getDirectionTo(adjPos);
                }
            }
        }

    }
}

const Pos& Snake::getHead() const {
    return *bodies.begin();
}

const Pos& Snake::getTail() const {
    return *bodies.rbegin();
}

void Snake::removeTail() {
    map->getPoint(getTail()).setType(Point::Type::EMPTY);
    bodies.pop_back();
    if (bodies.size() > 1) {
        map->getPoint(getTail()).setType(Point::Type::SNAKE_TAIL);
    }
}

void Snake::findMinPathToFood(list<Direction> &path) {
    findPathTo(0, map->getFood(), path);
}

void Snake::findMaxPathToTail(list<Direction> &path) {
    findPathTo(1, getTail(), path);
}

void Snake::findPathTo(const int pathType, const Pos &goal, list<Direction> &path) {
    Point::Type oriType = map->getPoint(goal).getType();
    map->getPoint(goal).setType(Point::Type::EMPTY);
    if (pathType == 0) {
        findMinPath(getHead(), goal, path);
    } else if (pathType == 1) {
        findMaxPath(getHead(), goal, path);
    }
    map->getPoint(goal).setType(oriType);  // Retore point type
}

void Snake::findMinPath(const Pos &from, const Pos &to, list<Direction> &path) {
    // Init
	//Matrix (Node) Initialization
    SizeType row = map->getRowCount();
	SizeType col = map->getColCount();
    for (SizeType i = 1; i < row - 1; ++i) {
        for (SizeType j = 1; j < col - 1; ++j) {
            map->getPoint(Pos(i, j)).setDist(Point::MAX_VALUE);
        }
    }
    path.clear();
    map->getPoint(from).setDist(0);
    queue<Pos> openList; // Creating Queues for Navigation
    openList.push(from); //Push start node into queue
    // BFS
    while (!openList.empty()) { //When the search is complete, the queue is empty (escape while statement)
        Pos curPos = openList.front();
        const Point &curPoint = map->getPoint(curPos);
        openList.pop();
        map->showPos(curPos);
        if (curPos == to) { //If the visited vertex is a destination, the search ends
            buildPath(from, to, path); //Return path
            break;
        }
        vector<Pos> adjPositions = curPos.getAllAdj();
        Random<>::getInstance()->shuffle(adjPositions.begin(), adjPositions.end());
        // Arrange the order of traversing to make the result path as straight as possible
       //Make the resulting path as straight as possible
        Direction bestDirec = (curPos == from ? direc : curPoint.getParent().getDirectionTo(curPos));
        for (SizeType i = 0; i < adjPositions.size(); ++i) {
            if (bestDirec == curPos.getDirectionTo(adjPositions[i])) {
                util::swap(adjPositions[0], adjPositions[i]);
                break;
            }
        }
        // Traverse the adjacent positions
		//Push Adjacent Nodes into a Queue
        for (const Pos &adjPos : adjPositions) {
            Point &adjPoint = map->getPoint(adjPos);
            if (map->isEmpty(adjPos) && adjPoint.getDist() == Point::MAX_VALUE) {
                adjPoint.setParent(curPos); //Save the current node to the Parent node (after which the parent node becomes the path)
                adjPoint.setDist(curPoint.getDist() + 1);
                openList.push(adjPos); //Push Adjacent Nodes into a Queue
            }
        }
    }
}

void Snake::findMaxPath(const Pos &from, const Pos &to, list<Direction> &path) {
    // Get the shortest path
    bool oriEnabled = map->isTestEnabled();//Activate the test.
    map->setTestEnabled(false); //Terminating the testMaxPath function
    findMinPath(from, to, path);
    map->setTestEnabled(oriEnabled); //Enable testing for the shortest path.
    // Init
    SizeType row = map->getRowCount();
	SizeType col = map->getColCount();
    for (SizeType i = 1; i < row - 1; ++i) { //Initialize map size and current location
        for (SizeType j = 1; j < col - 1; ++j) {
            map->getPoint(Pos(i, j)).setVisit(false); //Initialize function to know whether you visited
        }
    }
    // Make all points on the path visited
    Pos cur = from;
    for (const Direction d : path) {
        map->getPoint(cur).setVisit(true);
        cur = cur.getAdj(d); //The current cursor position returns the position adjacent to the specified direction.
    }
    map->getPoint(cur).setVisit(true);
    // Extend the path between each pair of the points
    for (auto it = path.begin(); it != path.end();) {
        if (it == path.begin()) {
            cur = from; //Initialize current location as starting point
        }
        bool extended = false;
        Direction curDirec = *it;
        Pos next = cur.getAdj(curDirec);//The next position returns the contiguous position in the direction of curDirec.
        switch (curDirec) {
            case LEFT:
            case RIGHT: { //the current direction points left or right
                Pos curUp = cur.getAdj(UP);
                Pos nextUp = next.getAdj(UP);
                // Check two points above
                if (map->isEmptyNotVisit(curUp) && map->isEmptyNotVisit(nextUp)) {
                    map->getPoint(curUp).setVisit(true);
                    map->getPoint(nextUp).setVisit(true);
                    it = path.erase(it);
                    it = path.insert(it, DOWN);
                    it = path.insert(it, curDirec);
                    it = path.insert(it, UP);
                    it = path.begin();
                    extended = true;
                } else {
                    Pos curDown = cur.getAdj(DOWN);
                    Pos nextDown = next.getAdj(DOWN);
                    // Check two points below
                    if (map->isEmptyNotVisit(curDown) && map->isEmptyNotVisit(nextDown)) {
                        map->getPoint(curDown).setVisit(true);
                        map->getPoint(nextDown).setVisit(true);
                        it = path.erase(it);
                        it = path.insert(it, UP);
                        it = path.insert(it, curDirec);
                        it = path.insert(it, DOWN);
                        it = path.begin();//Change path location back to first
                        extended = true;
                    }
                }
                break;
            }
            case UP:
            case DOWN: {//the current direction points up or down
                Pos curLeft = cur.getAdj(LEFT);
                Pos nextLeft = next.getAdj(LEFT);
                // Check two points on the left
                if (map->isEmptyNotVisit(curLeft) && map->isEmptyNotVisit(nextLeft)) {
                    map->getPoint(curLeft).setVisit(true);
                    map->getPoint(nextLeft).setVisit(true);
                    it = path.erase(it);
                    it = path.insert(it, RIGHT);
                    it = path.insert(it, curDirec);
                    it = path.insert(it, LEFT);
                    it = path.begin();
                    extended = true;
                } else {
                    Pos curRight = cur.getAdj(RIGHT);
                    Pos nextRight = next.getAdj(RIGHT);
                    // Check two points on the right
                    if (map->isEmptyNotVisit(curRight) && map->isEmptyNotVisit(nextRight)) {
                        map->getPoint(curRight).setVisit(true);
                        map->getPoint(nextRight).setVisit(true);
                        it = path.erase(it);
                        it = path.insert(it, LEFT);
                        it = path.insert(it, curDirec);
                        it = path.insert(it, RIGHT);
                        it = path.begin();
                        extended = true;
                    }
                }
                break;
            }
            default:
                break;
        }
        if (!extended) {
            ++it; // Path continues to increase
            cur = next;//Move to the next location
        }
    }
}

void Snake::buildPath(const Pos &from, const Pos &to, list<Direction> &path) const {
    Pos tmp = to;
	Pos parent;
    while (tmp != from) {
        parent = map->getPoint(tmp).getParent();
        path.push_front(parent.getDirectionTo(tmp));
        tmp = parent;
    }
}

void Snake::buildHamilton() {
    // Change the initial body to a wall temporarily
    Pos bodyPos = *(++bodies.begin());
    Point &bodyPoint = map->getPoint(bodyPos);
    map->getPoint(*(++bodies.begin())).setType(Point::Type::WALL);
    // Get the longest path
    bool oriEnabled = map->isTestEnabled();
    map->setTestEnabled(false);
    list<Direction> maxPath;
    findMaxPathToTail(maxPath);
    map->setTestEnabled(oriEnabled);
    bodyPoint.setType(Point::Type::SNAKE_BODY);
    // Initialize the first three incides of the cycle
    Point::ValueType index = 0;
    for (auto it = bodies.crbegin(); it != bodies.crend(); ++it) {
        map->getPoint(*it).setIndex(index++);
    }
    // Build remaining cycle
    SizeType size = map->getSize();
    Pos cur = getHead();
    for (const Direction d : maxPath) {
        Pos next = cur.getAdj(d);
        map->getPoint(next).setIndex((map->getPoint(cur).getIndex() + 1) % size);
        cur = next;
    }
}
