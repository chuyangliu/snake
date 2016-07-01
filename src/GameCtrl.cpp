#include "GameCtrl.h"
#include "Console.h"
#include <exception>
#include <cstdio>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif

const std::string GameCtrl::MSG_BAD_ALLOC = "Oops! Not enough memory to run the game! Press any key to continue...";
const std::string GameCtrl::MSG_LOSE = "Sorry! You lose! Press any key to continue...";
const std::string GameCtrl::MSG_WIN = "Congratulations! You Win! Press any key to continue...";

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
    mutexExit.lock();
    stopThreads();
    Console::setCursor(0, mapRowCnt + 1);
    Console::writeWithColor(msg, ConsoleColor(WHITE, BLACK, true, false));
    Console::getch();
    Console::setCursor(0, mapRowCnt + 2);
    release();
    mutexExit.unlock();
    exit(0);
}

void GameCtrl::moveSnake() {
    mutexMove.lock();
    if (snake->getMoveArea()->isFilledWithBody()) {
        // Unlock must outside the exitGame()
        // because exitGame() will terminate the program
        // and there is no chance to unlock the mutex
        // after exectuing exitGame().
        mutexMove.unlock();
        exitGame(MSG_WIN);
    } else if (snake->isDead()) {
        mutexMove.unlock();
        exitGame(MSG_LOSE);
    } else {
        mutexMove.unlock();
        snake->move();
    }
}

void GameCtrl::sleepFor(const long ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void GameCtrl::sleepByFPS() const {
    sleepFor(static_cast<long>((1.0 / fps) * 1000));
}

void GameCtrl::startDraw() {
    drawThread = new(std::nothrow) std::thread(&GameCtrl::draw, this);
    if (!drawThread) {
        exitGame(MSG_BAD_ALLOC);
    }
    drawThread->detach();
}

void GameCtrl::draw() const {
    Console::clear();
    while (threadWork) {
        // Drawing
        Console::setCursor();
        for (unsigned i = 0; i < snake->getMoveArea()->getRowCount() && threadWork; ++i) {
            for (unsigned j = 0; j < snake->getMoveArea()->getColCount() && threadWork; ++j) {
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
            if (threadWork) {
                printf("\n");
            }
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
    while (threadWork) {
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
    while (threadWork) {
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
    while (threadWork) {
        moveSnake();
        sleepFor(autoMoveInterval);
    }
}

void GameCtrl::startThreads() {
    startDraw();
    startCreateFood();

    if (enableKeyboard) {
        startKeyboardReceiver();
    }

    if (autoMoveSnake) {
        startAutoMove();
    }
}

void GameCtrl::stopThreads() {
    threadWork = false;
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
    if (n < 3) {
        mapRowCnt = 3;
    } else {
        mapRowCnt = n;
    }
}

void GameCtrl::setMapColumn(const unsigned &n) {
    if (n < 6) {
        mapColCnt = 6;
    } else {
        mapColCnt = n;
    }
}

int GameCtrl::random(const int min, const int max) {
    static bool set_seed = true;
    if (set_seed) srand(static_cast<unsigned>(time(NULL)));
    set_seed = false;
    return rand() % (max - min + 1) + min;
}
