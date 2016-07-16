#include "Map.h"
#include "GameCtrl.h"
#include "Convert.h"
#include <algorithm>

using std::vector;
using std::string;

Map::Map(const size_type &rowCnt_, const size_type &colCnt_)
    : content(rowCnt_, vector<SearchableGrid>(colCnt_)) {
    init();
}

Map::~Map() {
}

void Map::init() {
    auto rows = getRowCount(), cols = getColCount();
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            content[i][j].setLocation(Point(i, j));
        }
    }
    // Add default walls
    for (size_type i = 0; i < rows; ++i) {
        if (i == 0 || i == rows - 1) {  // The first and last row
            for (size_type j = 0; j < cols; ++j) {
                content[i][j].setType(Grid::GridType::WALL);
            }
        } else {  // Rows in the middle
            content[i][0].setType(Grid::GridType::WALL);
            content[i][cols - 1].setType(Grid::GridType::WALL);
        }
    }
}

SearchableGrid& Map::getGrid(const Point &p) {
    return content[p.getX()][p.getY()];
}

const SearchableGrid& Map::getGrid(const Point &p) const {
    return content[p.getX()][p.getY()];
}

bool Map::isUnsafe(const Point &p) const {
    return getGrid(p).getType() == Grid::GridType::WALL
        || getGrid(p).getType() == Grid::GridType::SNAKEHEAD1
        || getGrid(p).getType() == Grid::GridType::SNAKEHEAD2
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY1
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY2
        || getGrid(p).getType() == Grid::GridType::SNAKETAIL1
        || getGrid(p).getType() == Grid::GridType::SNAKETAIL2;
}

bool Map::isUnsearch(const Point &p) const {
    return getGrid(p).getType() == Grid::GridType::WALL
        || getGrid(p).getType() == Grid::GridType::SNAKEHEAD1
        || getGrid(p).getType() == Grid::GridType::SNAKEHEAD2
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY1
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY2
        || getGrid(p).getType() == Grid::GridType::SNAKETAIL1
        || getGrid(p).getType() == Grid::GridType::SNAKETAIL2
        || getGrid(p).getType() == Grid::GridType::FOOD;
}

bool Map::isInside(const Point &p) const {
    return p.getX() > 0 && p.getY() > 0
        && p.getX() < (Point::attr_type)getRowCount() - 1
        && p.getY() < (Point::attr_type)getColCount() - 1;
}

bool Map::isFilledWithBody() const {
    auto rows = getRowCount(), cols = getColCount();
    for (size_type i = 1; i < rows - 1; ++i) {
        for (size_type j = 1; j < cols - 1; ++j) {
            auto type = content[i][j].getType();
            if (!(type == Grid::GridType::SNAKEHEAD1
                || type == Grid::GridType::SNAKEBODY1
                || type == Grid::GridType::SNAKETAIL1
                || type == Grid::GridType::SNAKEHEAD2
                || type == Grid::GridType::SNAKEBODY2
                || type == Grid::GridType::SNAKETAIL2)) {
                return false;
            }
        }
    }
    return true;
}

void Map::getEmptyPoints(vector<Point> &points) const {
    points.clear();
    auto rows = getRowCount(), cols = getColCount();
    for (size_type i = 1; i < rows - 1; ++i) {
        for (size_type j = 1; j < cols - 1; ++j) {
            if (content[i][j].getType()
                == Grid::GridType::EMPTY) {
                points.push_back(Point(i, j));
            }
        }
    }
}

void Map::createFood() {
    if (isFilledWithBody()) {
        return;
    }
    vector<Point> points;
    getEmptyPoints(points);
    if (!points.empty()) {
        food = points[random(0, points.size() - 1)];
        content[food.getX()][food.getY()].setType(Grid::GridType::FOOD);
    }
}

void Map::removeFood() {
    if (food != Point::INVALID) {
        content[food.getX()][food.getY()].setType(Grid::GridType::EMPTY);
        food = Point::INVALID;
    }
}

bool Map::hasFood() const {
    return food != Point::INVALID;
}

Map::size_type Map::getRowCount() const {
    return content.size();
}

Map::size_type Map::getColCount() const {
    return content[0].size();
}

const Point& Map::getFood() const {
    return food;
}

void Map::setShowSearchDetails(const bool &b) {
    showSearchDetails = b;
}

unsigned Map::getManhattenDist(const Point &from, const Point &to) {
    auto dx = abs(from.getX() - to.getX());
    auto dy = abs(from.getY() - to.getY());
    return dx + dy;
}

unsigned Map::getGeometricDist(const Point &from, const Point &to) {
    auto dx = abs(from.getX() - to.getX());
    auto dy = abs(from.getY() - to.getY());
    return static_cast<unsigned>(sqrt(dx * dx + dy * dy));
}

void Map::showVisitedNodeIfNeeded(const Point &n, const Grid::GridType &type) {
    if (showSearchDetails) {
        getGrid(n).setType(type);
        GameCtrl::getInstance()->sleepFor(10);
    }
}

void Map::showPathIfNeeded(const Point &start, const std::list<Direction> &path) {
    if (showSearchDetails) {
        Point tmp = start;
        for (const auto &d : path) {
            showVisitedNodeIfNeeded(tmp, Grid::GridType::SNAKEHEAD1);
            tmp = tmp.getOneAdjPoint(d);
        }
        showVisitedNodeIfNeeded(tmp, Grid::GridType::SNAKEHEAD1);
    }
}

void Map::constructPath(const Point &from, const Point &to, std::list<Direction> &path) {
    path.clear();
    Point tmp = to, parent;
    while (tmp != Point::INVALID && tmp != from) {
        parent = getGrid(tmp).getParent();
        path.push_front(parent.getDirectionTo(tmp));
        tmp = parent;
    }
}

void Map::sortByH2(std::vector<Point> &points, const Point &goal) {
    std::sort(points.begin(), points.end(), [&](const Point &a, const Point &b) {
        auto h1 = estimateH2(a, goal);
        auto h2 = estimateH2(b, goal);
        return h1 > h2;
    });
}

void Map::randomChange(std::vector<Point> &points) {
    for (unsigned i = 1; i < points.size(); ++i) {
        auto random = Map::random(0, i);
        Point tmp = points[i];
        points[i] = points[random];
        points[random] = tmp;
    }
 }

int Map::random(const int min, const int max) {
    static bool setSeed = true;
    if (setSeed) srand(static_cast<unsigned>(time(NULL)));
    setSeed = false;
    return rand() % (max - min + 1) + min;
}

void Map::findMinPath(const Point &from, const Point &to, std::list<Direction> &path) {
    if (!isInside(from) || !isInside(to)) {
        return;
    }

    // Initialize g value
    auto rows = getRowCount(), cols = getColCount();
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            content[i][j].setG(GameCtrl::INF);
        }
    }

    // Create open list and close list
    min_heap openList;
    hash_table closeList(2 * rows * cols, Point::hash);

    // Create local variables in the loop to save allocation time
    SearchableGrid curGrid;
    Point curPoint;
    vector<Point> adjPoints(4, Point::INVALID);

    // Add first search node
    SearchableGrid &start = getGrid(from);
    start.setG(0);
    start.setH(estimateH1(start.getLocation(), to));
    openList.push(start);

    while (!openList.empty()) {

        // Loop until the open list is empty or finding
        // a node that is not in the close list.
        do {
            curGrid = openList.top();
            curPoint = curGrid.getLocation();
            openList.pop();
        } while (!openList.empty() && closeList.find(curPoint) != closeList.end());

        // If all the nodes on the map is in the close list,
        // then there is no available path between the two
        // nodes.
        if (openList.empty() && closeList.find(curPoint) != closeList.end()) {
            break;
        }

        showVisitedNodeIfNeeded(curPoint, Grid::GridType::SNAKEBODY1);

        if (curPoint == to) {
            constructPath(from, to, path);
            showPathIfNeeded(from, path);
            break;
        }

        closeList.insert(curPoint);
        curPoint.getAllAdjPoint(adjPoints);
        randomChange(adjPoints);  // Ensure randomly traversing
        for (const auto &adjPoint : adjPoints) {
            if (!isUnsearch(adjPoint) && closeList.find(adjPoint) == closeList.end()) {
                SearchableGrid &adjGrid = getGrid(adjPoint);
                // The cost of moving from one grid to its
                // adjacent grid is set to 1.
                if (curGrid.getG() + 1 < adjGrid.getG()) {
                    adjGrid.setParent(curPoint);
                    adjGrid.setG(curGrid.getG() + 1);
                    adjGrid.setH(estimateH1(adjGrid.getLocation(), to));
                    openList.push(adjGrid);
                }
            }
        }
    }
}

void Map::findMaxPath(const Point &from, const Point &to, std::list<Direction> &path) {
    if (!isInside(from) || !isInside(to)) {
        return;
    }
    hash_table closeList(2 * getRowCount() * getColCount(), Point::hash);
    dfsFindLongest(from, from, to, closeList, path);
    showPathIfNeeded(from, path);
}

void Map::dfsFindLongest(const Point &n,
              const Point &from,
              const Point &to,
              Map::hash_table &closeList,
              std::list<Direction> &path) {
    closeList.insert(n);
    showVisitedNodeIfNeeded(n, Grid::GridType::SNAKEBODY1);
    if (n == to) {
        constructPath(from, to, path);
    } else {
        // Start traversing with the farthest point.
        vector<Point> adjPoints(4, Point::INVALID);
        n.getAllAdjPoint(adjPoints);
        sortByH2(adjPoints, to);
        for (const auto &adj : adjPoints) {
            if (!isUnsearch(adj) && closeList.find(adj) == closeList.end()) {
                getGrid(adj).setParent(n);
                dfsFindLongest(adj, from, to, closeList, path);
            }
        }
    }
}

void Map::createMaze(const Point &start) {
    if (!isInside(start)) {
        throw std::range_error("Map.generateMaze(): The start point is not inside the map.");
    }

    auto rows = getRowCount(), cols = getColCount();
    if (rows % 2 == 0 || cols % 2 == 0) {
        throw std::range_error("Map.generateMaze(): The size of the map must be (odd number)*(odd number).");
    }

    if (rows < 5 || cols < 5) {
        throw std::range_error("Map.generateMaze(): Require minimum map size: 5*5");
    }

    // Build all walls
    for (size_type i = 2; i < rows - 1; i += 2) {
        for (size_type j = 1; j < cols - 1; ++j) {
            content[i][j].setType(Grid::GridType::WALL);
        }
    }
    for (size_type i = 1; i < rows; i += 2) {
        for (size_type j = 2; j < cols - 1; j += 2) {
            content[i][j].setType(Grid::GridType::WALL);
        }
    }

    // Break some walls to create maze
    hash_table closeList(2 * rows *cols, Point::hash);
    dfsBreakWalls(start, closeList);
}

void Map::dfsBreakWalls(const Point &n, Map::hash_table &closeList) {
    closeList.insert(n);
    vector<Point> adjWalls(4, Point::INVALID);
    n.getAllAdjPoint(adjWalls);
    randomChange(adjWalls);
    for (const auto &wall : adjWalls) {
        Direction direc = n.getDirectionTo(wall);
        Point p = wall.getOneAdjPoint(direc);
        if (isInside(p) && !isUnsearch(p)
            && closeList.find(p) == closeList.end()) {
            getGrid(wall).setType(Grid::GridType::EMPTY);  // Break one wall
            dfsBreakWalls(p, closeList);
        }
    }
}

SearchableGrid::value_type Map::estimateH1(const Point &from, const Point &to) const {
    auto m = getManhattenDist(from, to);
    auto g = getGeometricDist(from, to);
    return 1 * m + 1 * g;
}

SearchableGrid::value_type Map::estimateH2(const Point &from, const Point &to) const {
    auto m = getManhattenDist(from, to);
    auto g = getGeometricDist(from, to);
    return 1 * m + 0 * g;
}
