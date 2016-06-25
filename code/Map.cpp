#include "Map.h"
#include "GameCtrl.h"
#include <new>

Map::Map(const unsigned &rowCnt_, const unsigned &colCnt_) : rowCnt(rowCnt_), colCnt(colCnt_) {
    content = new(std::nothrow) Grid*[rowCnt_];
    if (!content) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
    for (unsigned i = 0; i < rowCnt_; ++i) {
        content[i] = new(std::nothrow) Grid[colCnt_];
        if (!content) {
            GameCtrl::exitWithException("Not enough memory.\n");
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
    for (unsigned i = 0; i < GameCtrl::MAP_ROW; ++i) {
        delete[] content[i];
        content[i] = nullptr;
    }
    delete[] content;
    content = nullptr;
}

unsigned Map::getRowCount() const {
    return rowCnt;
}

unsigned Map::getColCount() const {
    return colCnt;
}

Grid& Map::at(const unsigned &x, const unsigned &y) {
    return content[x][y];
}

const Grid& Map::at(const unsigned &x, const unsigned &y) const {
    return content[x][y];
}
