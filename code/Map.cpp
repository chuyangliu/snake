#include "Map.h"
#include "GameCtrl.h"
#include <new>

Map::Map() {
    content = new(std::nothrow) Grid*[GameCtrl::MAP_ROW];
    if (!content) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
    for (unsigned i = 0; i < GameCtrl::MAP_ROW; ++i) {
        content[i] = new(std::nothrow) Grid[GameCtrl::MAP_COL];
        if (!content) {
            GameCtrl::exitWithException("Not enough memory.\n");
        }
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
