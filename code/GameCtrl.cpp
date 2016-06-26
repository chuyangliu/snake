#include "GameCtrl.h"
#include "Console.h"
#include "Map.h"
#include <iostream>

using std::cout;
using std::endl;
using std::thread;

unsigned GameCtrl::MAP_ROW = 12;
unsigned GameCtrl::MAP_COL = 12;

GameCtrl* GameCtrl::getInstance() {
    // According to C++11, static field constructor is thread-safe
    static GameCtrl instance;
    return &instance;
}

GameCtrl::GameCtrl() {
    map = new(std::nothrow) Map(GameCtrl::MAP_ROW, GameCtrl::MAP_COL);
    if (!map) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
}

GameCtrl::~GameCtrl() {
    delete map;
    map = nullptr;

    delete redrawThread;
    redrawThread = nullptr;
}

int GameCtrl::start() {
    startDrawing();

    while (1) {}

    return 0;
}
 
void GameCtrl::exitWithException(const std::string &msg) {
    Console::writeWithColor(msg, FOREGROUND_RED | FOREGROUND_INTENSITY);
    redrawThread->join();
    exit(-1);
}

void GameCtrl::startDrawing() {
    redrawThread = new(std::nothrow) thread(&GameCtrl::draw, this);
    if (!redrawThread) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::draw() {
    Console::clear();
    while (1) {
        Console::setCursor();
        for (unsigned i = 0; i < map->getRowCount(); ++i) {
            for (unsigned j = 0; j < map->getColCount(); ++j) {
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
}
