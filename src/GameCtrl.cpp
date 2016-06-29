#include "GameCtrl.h"
#include "Console.h"
#include <exception>
#include <cstdio>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif

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
        exitWithException("Not enough memory.\n");
    }
}

GameCtrl::~GameCtrl() {
    release();
}

int GameCtrl::run() {
    try {
        startThreads();
        while (1) {

        }
        stopThreads();
        return 0;
    } catch (std::exception &e) {
        exitWithException(e.what());
        return -1;
    }
}
 
void GameCtrl::exitWithException(const std::string &msg) {
    stopThreads();
    release();
    Console::setCursor(0, MAP_ROW + 1);
    Console::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::getch();
    exit(-1);
}

void GameCtrl::sleepFor(const long ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void GameCtrl::sleepByFPS() const {
    sleepFor(static_cast<long>((1.0 / fps) * 1000));
}

void GameCtrl::setFPS(const double &fps_) {
    fps = fps_;
}

void GameCtrl::setAutoMoveSnake(const bool &move) {
    autoMoveSnake = move;
}

void GameCtrl::setEnableKeyboard(const bool &enable) {
    enableKeyboard = enable;
}

void GameCtrl::setAutoMoveInterval(const long &ms) {
    autoMoveInterval = ms;
}

void GameCtrl::startDrawing() {
    drawThread = new(std::nothrow) std::thread(&GameCtrl::draw, this);
    if (!drawThread) {
        exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::draw() const {
    Console::clear();
    while (threadRun) {
        Console::setCursor();
        for (unsigned i = 0; i < snake->getMoveArea()->getRowCount(); ++i) {
            for (unsigned j = 0; j < snake->getMoveArea()->getColCount(); ++j) {
                switch (snake->getMoveArea()->at(Point(i, j)).getType()) {
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
    keyboardThread = new(std::nothrow) std::thread(&GameCtrl::receiveKeyboardInstruction, this);
    if (!keyboardThread) {
        exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::receiveKeyboardInstruction() {
    while (threadRun) {
        if (Console::kbhit()) {
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
                default:
                    break;
            }
        }
        sleepByFPS();
    }
}

void GameCtrl::startCreateFood() {
    foodThread = new(std::nothrow)  std::thread(&GameCtrl::createFood, this);
    if (!foodThread) {
        exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::createFood() {
    while (threadRun) {
        if (!snake->getMoveArea()->hasFood()) {
            snake->getMoveArea()->createFood();
        }
        sleepByFPS();
    }
}

void GameCtrl::startAutoMove() {
    moveThread = new(std::nothrow)  std::thread(&GameCtrl::autoMove, this);
    if (!moveThread) {
        exitWithException("Not enough memory.\n");
    }
}

void GameCtrl::autoMove() {
    while (1) {
        snake->move();
        sleepFor(autoMoveInterval);
    }
}

void GameCtrl::startThreads() {
    startDrawing();
    startCreateFood();

    if (enableKeyboard) {
        startKeyboardReceiver();
    }

    if (autoMoveSnake) {
        startAutoMove();
    }
}

void GameCtrl::joinThreads() {
    if (drawThread && drawThread->joinable()) {
        drawThread->join();
    }

    if (keyboardThread && keyboardThread->joinable()) {
        keyboardThread->join();
    }

    if (foodThread && foodThread->joinable()) {
        foodThread->join();
    }

    if (moveThread && moveThread->joinable()) {
        moveThread->join();
    }
}

void GameCtrl::stopThreads() {
    threadRun = false;
    joinThreads();
}

void GameCtrl::release() {
    delete snake;
    delete drawThread;
    delete keyboardThread;
    delete foodThread;
    delete moveThread;
    snake = nullptr;
    drawThread = nullptr;
    keyboardThread = nullptr;
    foodThread = nullptr;
    moveThread = nullptr;
}
