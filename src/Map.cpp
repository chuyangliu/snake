#include "Map.h"
#include "GameCtrl.h"
#include <new>
#include <cstdlib>

Map::Map(const long &rowCnt_, const long &colCnt_) : rowCnt(rowCnt_), colCnt(colCnt_) {
    content = new(std::nothrow) Grid*[rowCnt_];
    if (!content) {
        GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
    }
    for (auto i = 0; i < rowCnt_; ++i) {
        content[i] = new(std::nothrow) Grid[colCnt_];
        if (!content) {
            GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
        }

        // Initial map content
        if (i == 0 || i == rowCnt - 1) {
            for (auto j = 0; j < colCnt; ++j) {
                content[i][j].setType(Grid::GridType::WALL);
            }
        }
        content[i][0].setType(Grid::GridType::WALL);
        content[i][colCnt - 1].setType(Grid::GridType::WALL);
    }
}

Map::~Map() {
    for (auto i = 0; i < rowCnt; ++i) {
        delete[] content[i];
        content[i] = nullptr;
    }
    delete[] content;
    content = nullptr;
    delete foodPos;
    foodPos = nullptr;
}

Grid& Map::at(const Point &p) {
    return content[p.getX()][p.getY()];
}

const Grid& Map::at(const Point &p) const {
    return content[p.getX()][p.getY()];
}

bool Map::isBodyOrBoundary(const Point &p) const {
    return p.getX() == 0 || p.getX() == rowCnt - 1 
        || p.getY() == 0 || p.getY() == colCnt - 1
        || at(p).getType() == Grid::GridType::SNAKEBODY;
}

bool Map::isInside(const Point &p) const {
    return p.getX() > 0 && p.getY() > 0
        && p.getX() < rowCnt - 1
        && p.getY() < colCnt - 1;
}

bool Map::isFilledWithBody() const {
    for (auto i = 1; i < rowCnt - 1; ++i) {
        for (auto j = 1; j < colCnt - 1; ++j) {
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
        row = GameCtrl::getInstance()->random(1, rowCnt - 2);
        col = GameCtrl::getInstance()->random(1, colCnt - 2);
    } while (content[row][col].getType() != Grid::GridType::EMPTY);

    if (!foodPos) {
        foodPos = new(std::nothrow) Point(row, col);
        if (!foodPos) {
            GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
        }
    } else {
        foodPos->setX(row);
        foodPos->setY(col);
    }
    content[row][col].setType(Grid::GridType::FOOD);
}

void Map::removeFood() {
    if (foodPos) {
        content[foodPos->getX()][foodPos->getY()].setType(Grid::GridType::EMPTY);
        delete foodPos;
        foodPos = nullptr;
    }
}

bool Map::hasFood() const {
    return foodPos != nullptr;
}

long Map::getRowCount() const {
    return rowCnt;
}

long Map::getColCount() const {
    return colCnt;
}

const Point* Map::getFoodPos() const {
    return foodPos;
}
