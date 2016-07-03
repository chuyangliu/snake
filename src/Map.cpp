#include "Map.h"
#include "GameCtrl.h"
#include <new>
#include <cstdlib>

Map::Map(const long &rowCnt_, const long &colCnt_)
    : content(rowCnt_, std::vector<Grid>(colCnt_)) {
    setDefaultWalls();
}

Map::~Map() {
}

void Map::setDefaultWalls() {
    auto rows = getRowCount();
    auto cols = getColCount();
    for (auto i = 0; i < getRowCount(); ++i) {
        if (i == 0 || i == rows - 1) {  // The first and last rows
            for (auto j = 0; j < cols; ++j) {
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
    return p.getX() == 0 || p.getX() == getRowCount() - 1 
        || p.getY() == 0 || p.getY() == getColCount() - 1
        || getGrid(p).getType() == Grid::GridType::SNAKEBODY;
}

bool Map::isInside(const Point &p) const {
    return p.getX() > 0 && p.getY() > 0
        && p.getX() < getRowCount() - 1
        && p.getY() < getColCount() - 1;
}

bool Map::isFilledWithBody() const {
    for (auto i = 1; i < getRowCount() - 1; ++i) {
        for (auto j = 1; j < getColCount() - 1; ++j) {
            auto type = content[i][j].getType();
            if (!(type == Grid::GridType::SNAKEBODY
                || type == Grid::GridType::SNAKEHEAD)) {
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

    int row, col;
    do {
        row = GameCtrl::getInstance()->random(1, getRowCount() - 2);
        col = GameCtrl::getInstance()->random(1, getColCount() - 2);
    } while (content[row][col].getType() != Grid::GridType::EMPTY);

    food.setX(row);
    food.setY(col);
    content[row][col].setType(Grid::GridType::FOOD);
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

long Map::getRowCount() const {
    return content.size();
}

long Map::getColCount() const {
    return content[0].size();
}

const Point& Map::getFood() const {
    return food;
}
