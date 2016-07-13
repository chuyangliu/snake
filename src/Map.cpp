#include "Map.h"
#include "GameCtrl.h"
#include <algorithm>

using std::vector;

Map::Map(const size_type &rowCnt_, const size_type &colCnt_)
    : content(rowCnt_, vector<SearchableGrid>(colCnt_)) {
    init();
}

Map::~Map() {
}

void Map::init() {
    auto rows = getRowCount(), cols = getColCount();
    
    // Set locations of each grid
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            content[i][j].setLocation(Point(i, j));
        }
    }

    // Set default walls
    for (size_type i = 0; i < rows; ++i) {
        if (i == 0 || i == rows - 1) {  // The first and last rows
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

void Map::createFood() {
    if (isFilledWithBody()) {
        return;
    }
    food = createFoodPos();
    content[food.getX()][food.getY()].setType(Grid::GridType::FOOD);
}

Point Map::createFoodPos() const {
    auto rows = getRowCount(), cols = getColCount();
    Point::attr_type r, c;

    do {
        r = Map::random(1, rows - 2);
        c = Map::random(1, cols - 2);
    } while (content[r][c].getType() != Grid::GridType::EMPTY);
    
    return Point(r, c);
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
    // Check validity
    if (!isInside(from) || !isInside(to)) {
        return;
    }

    // Initialize g value of each grid
    auto rows = getRowCount(), cols = getColCount();
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            content[i][j].setG(GameCtrl::INF);
        }
    }

    // Create open list
    min_heap openList;

    // Create close list
    // The first param is the number of buckets in the hash table
    // The second param is the hash function
    hash_table closeList(2 * getRowCount() * getColCount(), Point::hash);

    // Create local variables in the loop to save allocation time
    SearchableGrid curGrid;
    Point curPoint;
    vector<Point> adjPoints(4, Point::INVALID);

    // Add first search node
    SearchableGrid &start = getGrid(from);
    start.setG(0);
    start.setH(estimateH1(start.getLocation(), to));
    openList.push(start);

    // Begin searching
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

        // Show search details if needed
        showVisitedNodeIfNeeded(curPoint, Grid::GridType::SNAKEBODY1);

        // If the destination location is found,
        // construct the path and exit.
        if (curPoint == to) {
            constructPath(from, to, path);
            showPathIfNeeded(from, path);  // Show search details if needed
            break;
        }

        // Add current node to close list
        closeList.insert(curPoint);

        // Get adjacent points to scan.
        // Before traversing, randomly rearrange the adjacent
        // points array to ensure randomness in traversing
        curPoint.getAllAdjPoint(adjPoints);
        randomChange(adjPoints);
        for (const auto &adjPoint : adjPoints) {
            if (!isUnsearch(adjPoint) && closeList.find(adjPoint) == closeList.end()) {
                SearchableGrid &adjGrid = getGrid(adjPoint);
                // If shorter path exists, update g, h, parent field and add 
                // the adjacent grid to the open list. The cost of moving from
                // one grid to its adjacent grid is set to 1.
                if (curGrid.getG() + 1 < adjGrid.getG()) {
                    adjGrid.setParent(curPoint);  // Record path
                    adjGrid.setG(curGrid.getG() + 1);
                    adjGrid.setH(estimateH1(adjGrid.getLocation(), to));
                    openList.push(adjGrid);
                }
            }
        }
    }
}

void Map::findMaxPath(const Point &from, const Point &to, std::list<Direction> &path) {
    // Check validity
    if (!isInside(from) || !isInside(to)) {
        return;
    }

    // Create close list
    // The first param is the number of buckets in the hash table
    // The second param is the hash function
    hash_table closeList(2 * getRowCount() * getColCount(), Point::hash);

    // Begin searching
    dfs(from, from, to, closeList, path);

    // Show search details if needed
    showPathIfNeeded(from, path);
}

void Map::dfs(const Point &n,
              const Point &from,
              const Point &to,
              Map::hash_table &closeList,
              std::list<Direction> &path) {
    // Add current to close list
    closeList.insert(n);

    // Show search details if needed
    showVisitedNodeIfNeeded(n, Grid::GridType::SNAKEBODY1);

    // Begin searching
    if (n == to) {
        constructPath(from, to, path);
    } else {
        // Traverse adjacent points.
        // Start with the farthest point.
        vector<Point> adjPoints(4, Point::INVALID);
        n.getAllAdjPoint(adjPoints);
        sortByH2(adjPoints, to);
        for (const auto &adj : adjPoints) {
            if (!isUnsearch(adj) && closeList.find(adj) == closeList.end()) {
                getGrid(adj).setParent(n);  // Record path
                dfs(adj, from, to, closeList, path);
            }
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
