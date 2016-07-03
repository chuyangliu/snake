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
}

int GameCtrl::run() {
    try {
        initMap();
        initSnake();
        startThreads();

        while (1) {
            
        }

        //stopThreads();
        return 0;
    } catch (std::exception &e) {
        exitGame(e.what());
        return -1;
    }
}

void GameCtrl::initMap() {
    map = std::make_shared<Map>(mapRowCnt, mapColCnt);
    if (!map) {
        exitGame(MSG_BAD_ALLOC);
    }
}

void GameCtrl::initSnake() {
    snake.setMap(map);
    snake.addBody(Point(1, 3));
    snake.addBody(Point(1, 2));
    snake.addBody(Point(1, 1));
}

void GameCtrl::exitGame(const std::string &msg) {
    mutexExit.lock();
    stopThreads();
    Console::setCursor(0, mapRowCnt + 1);
    Console::writeWithColor(msg, ConsoleColor(WHITE, BLACK, true, false));
    Console::getch();
    Console::setCursor(0, mapRowCnt + 2);
    mutexExit.unlock();
    exit(0);
}

void GameCtrl::moveSnake() {
    mutexMove.lock();
    if (map->isFilledWithBody()) {
        // Unlock must outside the exitGame()
        // because exitGame() will terminate the program
        // and there is no chance to unlock the mutex
        // after exectuing exitGame().
        // Notice that exitGame() is a thread-safe method.
        mutexMove.unlock();
        exitGame(MSG_WIN);
    } else if (snake.isDead()) {
        mutexMove.unlock();
        exitGame(MSG_LOSE);
    } else {
        snake.move();
        mutexMove.unlock();
    }
}

void GameCtrl::sleepFor(const long ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void GameCtrl::sleepByFPS() const {
    sleepFor(static_cast<long>((1.0 / fps) * 1000));
}

void GameCtrl::draw() const {
    Console::clear();
    while (threadWork) {
        // Drawing
        Console::setCursor();
        auto rows = map->getRowCount();
        auto cols = map->getColCount();
        for (Map::size_type i = 0; i < rows && threadWork; ++i) {
            for (Map::size_type j = 0; j < cols && threadWork; ++j) {
                switch (map->getGrid(Point(i, j)).getType()) {
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

void GameCtrl::keyboard() {
    while (threadWork) {
        if (Console::kbhit()) {  // When keyboard is hit
            switch (Console::getch()) {
                case 'w':
                    keyboardMove(Snake::Direction::UP);
                    break;
                case 'a':
                    keyboardMove(Snake::Direction::LEFT);
                    break;
                case 's':
                    keyboardMove(Snake::Direction::DOWN);
                    break;
                case 'd':
                    keyboardMove(Snake::Direction::RIGHT);
                    break;
                default:
                    break;
            }
        }
        sleepByFPS();
    }
}

void GameCtrl::keyboardMove(const Snake::Direction &d) {
    if (autoMoveSnake && snake.getDirection() == d) {
        moveSnake();  // Accelerate the movements
    }
    snake.setDirection(d);
    if (!autoMoveSnake) {
        moveSnake();
    }
}

void GameCtrl::createFood() {
    while (threadWork) {
        if (!map->hasFood()) {
            map->createFood();
        }
        sleepByFPS();
    }
}

void GameCtrl::autoMove() {
    while (threadWork) {
        moveSnake();
        sleepFor(autoMoveInterval);
    }
}

void GameCtrl::startThreads() {
    // Detach each thread make each 
    // thread don't need to be joined
    drawThread = std::thread(&GameCtrl::draw, this);
    drawThread.detach();

    foodThread = std::thread(&GameCtrl::createFood, this);
    foodThread.detach();

    if (enableKeyboard) {
        keyboardThread = std::thread(&GameCtrl::keyboard, this);
        keyboardThread.detach();
    }

    if (autoMoveSnake) {
        autoMoveThread = std::thread(&GameCtrl::autoMove, this);
        autoMoveThread.detach();
    }
}

void GameCtrl::stopThreads() {
    threadWork = false;
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

void GameCtrl::setMapRow(const Map::size_type &n) {
    if (n < 3) {
        mapRowCnt = 3;
    } else {
        mapRowCnt = n;
    }
}

void GameCtrl::setMapColumn(const Map::size_type &n) {
    if (n < 6) {
        mapColCnt = 6;
    } else {
        mapColCnt = n;
    }
}

int GameCtrl::random(const int min, const int max) {
    static bool setSeed = true;
    if (setSeed) srand(static_cast<unsigned>(time(NULL)));
    setSeed = false;
    return rand() % (max - min + 1) + min;
}
