#include "Map.h"
#include "GameCtrl.h"
#include <new>

Map::Map(const unsigned &rowNum_, const unsigned &colNum_) : rowNum(rowNum_), colNum(colNum_) {
    content = new(std::nothrow) Grid*[rowNum_];
    if (!content) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
    for (unsigned i = 0; i < rowNum_; ++i) {
        content[i] = new(std::nothrow) Grid[colNum_];
        if (!content) {
            GameCtrl::exitWithException("Not enough memory.\n");
        }

        // Initial map content
        if (i == 0 || i == rowNum - 1) {
            for (unsigned j = 0; j < colNum; ++j) {
                content[i][j].setType(Grid::GridType::WALL);
            }
        }
        content[i][0].setType(Grid::GridType::WALL);
        content[i][colNum - 1].setType(Grid::GridType::WALL);
    }
}

Map::~Map() {
    for (unsigned i = 0; i < GameCtrl::MAP_ROW; ++i) {
        delete[] content[i];
        content[i] = nullptr;
    }
    delete[] content;
    content = nullptr;
}

unsigned Map::getRowNum() const {
    return rowNum;
}

unsigned Map::getColNum() const {
    return colNum;
}

Grid& Map::at(const unsigned &x, const unsigned &y) {
    return content[x][y];
}

const Grid& Map::at(const unsigned &x, const unsigned &y) const {
    return content[x][y];
}
