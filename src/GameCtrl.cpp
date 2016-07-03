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
        initSnakes();
        startThreads();
        while (1) {
            
        }
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

void GameCtrl::initSnakes() {
    snake1.setHeadType(Grid::GridType::SNAKEHEAD1);
    snake1.setBodyType(Grid::GridType::SNAKEBODY1);
    snake1.setMap(map);
    snake1.addBody(Point(1, 3));
    snake1.addBody(Point(1, 2));
    snake1.addBody(Point(1, 1));

    if (enableSecondSnake) {
        snake2.setHeadType(Grid::GridType::SNAKEHEAD2);
        snake2.setBodyType(Grid::GridType::SNAKEBODY2);
        snake2.setMap(map);
        snake2.addBody(Point(3, 3));
        snake2.addBody(Point(3, 2));
        snake2.addBody(Point(3, 1));
    }
}

void GameCtrl::exitGame(const std::string &msg) {
    mutexExit.lock();
    stopThreads();
    sleepFor(100);  // Wait draw thread to finish last drawing
    Console::setCursor(0, mapRowCnt + 6);
    Console::writeWithColor(msg, ConsoleColor(WHITE, BLACK, true, false));
    Console::getch();
    Console::setCursor(0, mapRowCnt + 7);
    mutexExit.unlock();
    exit(0);
}

void GameCtrl::moveSnake(Snake &s) {
    mutexMove.lock();
    if (map->isFilledWithBody()) {
        // Unlock must outside the exitGame()
        // because exitGame() will terminate the program
        // and there is no chance to unlock the mutex
        // after exectuing exitGame().
        // Notice that exitGame() is a thread-safe method.
        mutexMove.unlock();
        exitGame(MSG_WIN);
    } else if (s.isDead()) {
        mutexMove.unlock();
        exitGame(MSG_LOSE);
    } else {
        s.move();
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
        drawMapContent();
        drawGameInfo();
        sleepByFPS();
    }
}

void GameCtrl::drawMapContent() const {
    Console::setCursor();
    auto rows = map->getRowCount();
    auto cols = map->getColCount();
    for (Map::size_type i = 0; i < rows; ++i) {
        for (Map::size_type j = 0; j < cols; ++j) {
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
                case Grid::GridType::SNAKEHEAD1:
                    Console::writeWithColor("  ", ConsoleColor(RED, RED, true, true));
                    break;
                case Grid::GridType::SNAKEHEAD2:
                    Console::writeWithColor("  ", ConsoleColor(BLUE, BLUE, true, true));
                    break;
                case Grid::GridType::SNAKEBODY1:
                    Console::writeWithColor("  ", ConsoleColor(GREEN, GREEN, true, true));
                    break;
                case Grid::GridType::SNAKEBODY2:
                    Console::writeWithColor("  ", ConsoleColor(CYAN, CYAN, true, true));
                    break;
                default:
                    break;
            }
        }
        Console::write("\n");
    }
}

void GameCtrl::drawGameInfo() const {
    Console::setCursor(0, mapRowCnt + 1);
    Console::writeWithColor("Control:\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::writeWithColor("         Up  Left  Down  Right\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::writeWithColor("Snake1:  W   A     S     D\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::writeWithColor("Snake2:  I   J     K     L\n", ConsoleColor(WHITE, BLACK, true, false));
}

void GameCtrl::keyboard() {
    while (threadWork) {
        if (Console::kbhit()) {  // When keyboard is hit
            switch (Console::getch()) {
                case 'w':
                    keyboardMove(snake1, Snake::Direction::UP);
                    break;
                case 'a':
                    keyboardMove(snake1, Snake::Direction::LEFT);
                    break;
                case 's':
                    keyboardMove(snake1, Snake::Direction::DOWN);
                    break;
                case 'd':
                    keyboardMove(snake1, Snake::Direction::RIGHT);
                    break;
                case 'i':
                    keyboardMove(snake2, Snake::Direction::UP);
                    break;
                case 'j':
                    keyboardMove(snake2, Snake::Direction::LEFT);
                    break;
                case 'k':
                    keyboardMove(snake2, Snake::Direction::DOWN);
                    break;
                case 'l':
                    keyboardMove(snake2, Snake::Direction::RIGHT);
                    break;
                default:
                    break;
            }
        }
        sleepByFPS();
    }
}

void GameCtrl::keyboardMove(Snake &s, const Snake::Direction &d) {
    if (autoMoveSnake && s.getDirection() == d) {
        moveSnake(s);  // Accelerate the movements
    }
    s.setDirection(d);
    if (!autoMoveSnake) {
        moveSnake(s);
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
        moveSnake(snake1);
        moveSnake(snake2);
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

    keyboardThread = std::thread(&GameCtrl::keyboard, this);
    keyboardThread.detach();

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

void GameCtrl::setEnableSecondSnake(const bool &enable) {
    enableSecondSnake = enable;
}

void GameCtrl::setAutoMoveInterval(const long &ms) {
    autoMoveInterval = ms;
}

void GameCtrl::setMapRow(const Map::size_type &n) {
    if (n < 5) {
        mapRowCnt = 5;
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
