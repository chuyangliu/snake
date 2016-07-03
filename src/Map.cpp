#include "Map.h"
#include "GameCtrl.h"

Map::Map(const size_type &rowCnt_, const size_type &colCnt_)
    : content(rowCnt_, std::vector<Grid>(colCnt_)) {
    setDefaultWalls();
}

Map::~Map() {
}

void Map::setDefaultWalls() {
    auto rows = getRowCount(), cols = getColCount();
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

Grid& Map::getGrid(const Point &p) {
    return content[p.getX()][p.getY()];
}

const Grid& Map::getGrid(const Point &p) const {
    return content[p.getX()][p.getY()];
}

bool Map::isBodyOrBoundary(const Point &p) const {
    return getGrid(p).getType() == Grid::GridType::WALL
        || getGrid(p).getType() == Grid::GridType::SNAKEHEAD1
        || getGrid(p).getType() == Grid::GridType::SNAKEHEAD2
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY1
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY2;
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
            if (!(type == Grid::GridType::SNAKEBODY1
                || type == Grid::GridType::SNAKEHEAD1)) {
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
    food = generateFoodPos();
    content[food.getX()][food.getY()].setType(Grid::GridType::FOOD);
}

Point Map::generateFoodPos() const {
    auto rows = getRowCount(), cols = getColCount();
    Point::attr_type r, c;

    do {
        r = GameCtrl::getInstance()->random(1, rows - 2);
        c = GameCtrl::getInstance()->random(1, cols - 2);
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
