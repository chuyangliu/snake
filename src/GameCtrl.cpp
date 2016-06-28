#include "GameCtrl.h"
#include "Console.h"
#include <exception>
#include <cstdio>
#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif

using std::thread;
using std::exception;

unsigned GameCtrl::MAP_ROW = 20;
unsigned GameCtrl::MAP_COL = 20;

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
    release();
}

void GameCtrl::release() {
    delete snake;
    snake = nullptr;
    delete redrawThread;
    redrawThread = nullptr;
    delete keyboardThread;
    keyboardThread = nullptr;
}

int GameCtrl::start() {
    try {
        startDrawing();
        startKeyboardReceiver();
        while (1) {
            //snake->getMoveArea()->clearFood();
            //if (!snake->getMoveArea()->hasFood()) {
                //snake->getMoveArea()->createFood();
            //}
            //sleep_(20);

            //snake->setMoveDirection(Snake::MoveDirection::RIGHT);
            snake->move();
            sleep_(200);
        }

        redrawThread->join();
        keyboardThread->join();

        return 0;
    } catch (exception &e) {
        printf("%s\n", e.what());
        return -1;
    }
}
 
void GameCtrl::exitWithException(const std::string &msg) {
    Console::clear();
    Console::writeWithColor(msg, ConsoleColor(RED, BLACK, true, true));
    if (redrawThread) {
        redrawThread->join();
    }
    release();
    exit(-1);
}

void GameCtrl::sleep_(const long time) const {
#ifdef __linux__ 
    usleep(time * 1000);
#elif _WIN32
    Sleep(time);
#else
    // Other platform
#endif
}

void GameCtrl::sleepByFPS() const {
    sleep_(static_cast<long>(fps * 1000));
}

void GameCtrl::setFPS(const double &fps_) {
    fps = fps_;
}

void GameCtrl::startDrawing() {
    redrawThread = new(std::nothrow) thread(&GameCtrl::draw, this);
    if (!redrawThread) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::draw() const {
    Console::clear();
    while (1) {
        Console::setCursor();
        for (unsigned i = 0; i < snake->getMoveArea()->getRowCount(); ++i) {
            for (unsigned j = 0; j < snake->getMoveArea()->getColCount(); ++j) {
                switch (snake->getMoveArea()->at(i, j).getType()) {
                    case Grid::GridType::EMPTY:
                        Console::writeWithColor("  ", ConsoleColor(BLACK, BLACK));
                        break;
                    case Grid::GridType::WALL:
                        Console::writeWithColor("  ", ConsoleColor(WHITE, WHITE, true, true));
                        break;
                    case Grid::GridType::FOOD:
                        Console::writeWithColor("  ", ConsoleColor(YELLOW, YELLOW, true, true));
                        break;
                    case Grid::GridType::SNAKEHEAD:
                        Console::writeWithColor("  ", ConsoleColor(RED, RED, true, true));
                        break;
                    case Grid::GridType::SNAKEBODY:
                        Console::writeWithColor("  ", ConsoleColor(GREEN, GREEN, true, true));
                        break;
                    default:
                        break;
                }
            }
            printf("\n");
        }
        sleepByFPS();
    }
}

void GameCtrl::startKeyboardReceiver() {
    keyboardThread = new(std::nothrow) thread(&GameCtrl::receiveKeyboardInstruction, this);
    if (!keyboardThread) {
        GameCtrl::exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::receiveKeyboardInstruction() {
    while (1) {
        char c = Console::getch();
        switch (c) {
            case 'w':
                snake->setMoveDirection(Snake::MoveDirection::UP); break;
            case 'a':
                snake->setMoveDirection(Snake::MoveDirection::LEFT); break;
            case 's':
                snake->setMoveDirection(Snake::MoveDirection::DOWN); break;
            case 'd':
                snake->setMoveDirection(Snake::MoveDirection::RIGHT); break;
            case 27:  // Esc
            default:
                break;
        }
        sleepByFPS();
    }
}
