#include "GameCtrl.h"
#include "Console.h"
#include <exception>
#include <cstdio>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif

const std::string GameCtrl::MSG_BAD_ALLOC = "Oops! Not enough memory to run the game!\nPress any key to continue...\n";
const std::string GameCtrl::MSG_LOSE = "Sorry! You lose!\nPress any key to continue...\n";
const std::string GameCtrl::MSG_WIN = "Congratulations! You Win!\nPress any key to continue...\n";

GameCtrl* GameCtrl::getInstance() {
    // According to C++11, static field constructor is thread-safe
    static GameCtrl instance;
    return &instance;
}

GameCtrl::GameCtrl() {
}

GameCtrl::~GameCtrl() {
    release();
}

int GameCtrl::run() {
    try {
        createSnake();
        startThreads();
        while (1) {
            //// Check game over
            //if (snake->getMoveArea()->isFull()) {
            //    exitGame(MSG_WIN);
            //} else if (snake->isDead()) {
            //    exitGame(MSG_LOSE);
            //}

            ///*
            //Sleep time must be much longer than FPS since when exitGame()
            //called, all the threads will be stopped and the last movement of the
            //snake may never be drawn on the screen. Thus may mislead the user.
            //*/
            //sleepByFPS();
        }
        stopThreads();
        return 0;
    } catch (std::exception &e) {
        exitGame(e.what());
        return -1;
    }
}

void GameCtrl::createSnake() {
    snake = new(std::nothrow) Snake(mapRowCnt, mapColCnt);
    if (!snake) {
        exitGame(MSG_BAD_ALLOC);
    }
}

void GameCtrl::exitGame(const std::string &msg) {
    stopThreads();
    release();
    Console::setCursor(0, mapRowCnt + 1);
    Console::writeWithColor(msg, ConsoleColor(WHITE, BLACK, true, false));
    Console::getch();
    exit(-1);
}

void GameCtrl::moveSnake() {
    snake->move();
    //if (snake->getMoveArea()->isFull()) {
    //    exitGame(MSG_WIN);
    //} else if (snake->isDead()) {
    //    exitGame(MSG_LOSE);
    //}
}

void GameCtrl::sleepFor(const long ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void GameCtrl::sleepByFPS() const {
    sleepFor(static_cast<long>((1.0 / fps) * 1000));
}

void GameCtrl::startDrawing() {
    drawThread = new(std::nothrow) std::thread(&GameCtrl::draw, this);
    if (!drawThread) {
        exitGame(MSG_BAD_ALLOC);
    }
    drawThread->detach();
}

void GameCtrl::draw() const {
    Console::clear();
    while (threadWorking) {
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
        exitGame(MSG_BAD_ALLOC);
    }
    keyboardThread->detach();
}

void GameCtrl::receiveKeyboardInstruction() {
    while (threadWorking) {
        if (Console::kbhit()) {  // When keyboard is hit
            switch (Console::getch()) {
                case 'w':
                    if (autoMoveSnake && snake->getMoveDirection() == Snake::MoveDirection::UP) {
                        moveSnake();
                    }
                    snake->setMoveDirection(Snake::MoveDirection::UP);
                    break;
                case 'a':
                    if (autoMoveSnake && snake->getMoveDirection() == Snake::MoveDirection::LEFT) {
                        moveSnake();
                    }
                    snake->setMoveDirection(Snake::MoveDirection::LEFT);
                    break;
                case 's':
                    if (autoMoveSnake && snake->getMoveDirection() == Snake::MoveDirection::DOWN) {
                        moveSnake();
                    }
                    snake->setMoveDirection(Snake::MoveDirection::DOWN);
                    break;
                case 'd':
                    if (autoMoveSnake && snake->getMoveDirection() == Snake::MoveDirection::RIGHT) {
                        moveSnake();
                    }
                    snake->setMoveDirection(Snake::MoveDirection::RIGHT);
                    break;
                default:
                    continue;
            }
            if (!autoMoveSnake) {
                moveSnake();
            }
        }
        sleepByFPS();
    }
}

void GameCtrl::startCreateFood() {
    foodThread = new(std::nothrow)  std::thread(&GameCtrl::createFood, this);
    if (!foodThread) {
        exitGame(MSG_BAD_ALLOC);
    }
    foodThread->detach();
}

void GameCtrl::createFood() {
    while (threadWorking) {
        if (!snake->getMoveArea()->hasFood()) {
            snake->getMoveArea()->createFood();
        }
        sleepByFPS();
    }
}

void GameCtrl::startAutoMove() {
    moveThread = new(std::nothrow)  std::thread(&GameCtrl::autoMove, this);
    if (!moveThread) {
        exitGame(MSG_BAD_ALLOC);
    }
    moveThread->detach();
}

void GameCtrl::autoMove() {
    while (threadWorking) {
        moveSnake();
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

//void GameCtrl::joinThreads() {
//    if (drawThread && drawThread->joinable()) {
//        drawThread->join();
//    }
//
//    if (keyboardThread && keyboardThread->joinable()) {
//        keyboardThread->join();
//    }
//
//    if (foodThread && foodThread->joinable()) {
//        foodThread->join();
//    }
//
//    if (moveThread && moveThread->joinable()) {
//        moveThread->join();
//    }
//}

void GameCtrl::stopThreads() {
    threadWorking = false;
    //joinThreads();
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

void GameCtrl::setMapRow(const unsigned &n) {
    mapRowCnt = n;
}

void GameCtrl::setMapColumn(const unsigned &n) {
    mapColCnt = n;
}
