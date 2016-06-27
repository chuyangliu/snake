#include "GameCtrl.h"
#include "Console.h"
#include <iostream>
#include <exception>
#ifdef __linux__ 
#include <unistd.h>
#elif _WIN32
#include <Windows.h>
#endif

using std::cout;
using std::endl;
using std::thread;
using std::exception;

unsigned GameCtrl::MAP_ROW = 12;
unsigned GameCtrl::MAP_COL = 12;

GameCtrl* GameCtrl::getInstance() {
    // According to C++11, static field constructor is thread-safe
    static GameCtrl instance;
    return &instance;
}

GameCtrl::GameCtrl() {
    snake = new(std::nothrow) Snake();
    if (!snake) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
}

GameCtrl::~GameCtrl() {
    delete snake;
    snake = nullptr;
    delete redrawThread;
    redrawThread = nullptr;
}

int GameCtrl::start() {
    try {
        startDrawing();
        while (1) {
            //snake->getMoveArea()->clearFood();
            //if (!snake->getMoveArea()->hasFood()) {
                snake->getMoveArea()->createFood();
            //}
            sleep_(20);
        }
        return 0;
    } catch (exception &e) {
        cout << e.what() << endl;
        return -1;
    }
}
 
void GameCtrl::exitWithException(const std::string &msg) {
    Console::clear();
    Console::writeWithColor(msg, ConsoleColor(RED, BLACK, true, true));
    if (redrawThread) {
        redrawThread->join();
    }
    delete snake;
    snake = nullptr;
    delete redrawThread;
    redrawThread = nullptr;
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
        for (unsigned i = 0; i < snake->getMoveArea()->getRowCount(); ++i) {
            for (unsigned j = 0; j < snake->getMoveArea()->getColCount(); ++j) {
                switch (snake->getMoveArea()->at(i, j).getType()) {
                    case Grid::GridType::BLANK:
                        Console::writeWithColor("  ", ConsoleColor(BLACK, BLACK));
                        break;
                    case Grid::GridType::WALL:
                        Console::writeWithColor("  ", ConsoleColor(WHITE, WHITE, true, true));
                        break;
                    case Grid::GridType::FOOD:
                        Console::writeWithColor("  ", ConsoleColor(YELLOW, YELLOW, true, true));
                        break;
                    default:
                        break;
                }
            }
            cout << endl;
        }
    }
}

void GameCtrl::sleep_(const int time) {
#ifdef __linux__ 
    usleep(time * 1000);
#elif _WIN32
    Sleep(time);
#else
    // Other platform
#endif
}
