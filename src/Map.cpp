#include "Map.h"
#include "GameCtrl.h"
#include <new>
#include <cstdlib>

Map::Map(const unsigned &rowCnt_, const unsigned &colCnt_) : rowCnt(rowCnt_), colCnt(colCnt_) {
    content = new(std::nothrow) Grid*[rowCnt_];
    if (!content) {
        GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
    }
    for (unsigned i = 0; i < rowCnt_; ++i) {
        content[i] = new(std::nothrow) Grid[colCnt_];
        if (!content) {
            GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
        }

        // Initial map content
        if (i == 0 || i == rowCnt - 1) {
            for (unsigned j = 0; j < colCnt; ++j) {
                content[i][j].setType(Grid::GridType::WALL);
            }
        }
        content[i][0].setType(Grid::GridType::WALL);
        content[i][colCnt - 1].setType(Grid::GridType::WALL);
    }
}

Map::~Map() {
    for (unsigned i = 0; i < rowCnt; ++i) {
        delete[] content[i];
        content[i] = nullptr;
    }
    delete[] content;
    content = nullptr;
    delete foodPos;
    foodPos = nullptr;
}

Grid& Map::at(const Point &p) {
    return content[p.x][p.y];
}

const Grid& Map::at(const Point &p) const {
    return content[p.x][p.y];
}

bool Map::hitBodyOrBoundary(const Point &p) const {
    return p.x == 0 || p.x == rowCnt - 1 || p.y == 0 || p.y == colCnt - 1
        || at(p).getType() == Grid::GridType::SNAKEBODY;
}

bool Map::isInside(const Point &p) const {
    return p.x > 0 && p.y > 0 && p.x < rowCnt - 1 && p.y < colCnt - 1;
}

bool Map::isFull() const {
    for (unsigned i = 0; i < rowCnt; ++i) {
        for (unsigned j = 0; j < colCnt; ++j) {
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
    int row, col;
    do {
        row = random(1, rowCnt - 2);
        col = random(1, colCnt - 2);
    } while (content[row][col].getType() != Grid::GridType::EMPTY);

    if (!foodPos) {
        foodPos = new(std::nothrow) Point(row, col);
        if (!foodPos) {
            GameCtrl::getInstance()->exitGame(GameCtrl::MSG_BAD_ALLOC);
        }
    } else {
        foodPos->x = row;
        foodPos->y = col;
    }
    content[row][col].setType(Grid::GridType::FOOD);
}

void Map::removeFood() {
    if (foodPos) {
        content[foodPos->x][foodPos->y].setType(Grid::GridType::EMPTY);
        delete foodPos;
        foodPos = nullptr;
    }
}

bool Map::hasFood() const {
    return foodPos != nullptr;
}

unsigned Map::getRowCount() const {
    return rowCnt;
}

unsigned Map::getColCount() const {
    return colCnt;
}

const Point* Map::getFoodPos() const {
    return foodPos;
}

int Map::random(const int min, const int max) {
    static bool set_seed = true;
    if (set_seed) srand(static_cast<unsigned>(time(NULL)));
    set_seed = false;
    return rand() % (max - min + 1) + min;
}
