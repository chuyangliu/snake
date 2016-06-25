#include "GameCtrl.h"
#include "Console.h"
#include "Map.h"
#include <iostream>
using namespace std;

unsigned GameCtrl::MAP_ROW = 12;
unsigned GameCtrl::MAP_COL = 12;

GameCtrl::GameCtrl() {
    map = new(std::nothrow) Map(GameCtrl::MAP_ROW, GameCtrl::MAP_COL);
    if (!map) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
}

GameCtrl::~GameCtrl() {
    delete map;
}

int GameCtrl::start() {

    // Print map
    Console::clear();
    while (1) {
        Console::setCursor();
        for (unsigned i = 0; i < map->getRowNum(); ++i) {
            for (unsigned j = 0; j < map->getColNum(); ++j) {
                switch (map->at(i, j).getType()) {
                    case Grid::GridType::BLANK:
                        Console::writeWithColor("  ", 0);
                        break;
                    case Grid::GridType::WALL:
                        Console::writeWithColor("  ", BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
                        break;
                    default:
                        break;
                }
            }
            cout << endl;
        }
    }

    return 0;
}
 
void GameCtrl::exitWithException(const std::string &msg) {
    Console::writeWithColor(msg, FOREGROUND_RED | FOREGROUND_INTENSITY);
    exit(-1);
}
