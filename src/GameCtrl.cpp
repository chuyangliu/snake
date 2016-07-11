#include "GameCtrl.h"
#include "Console.h"
#include "Convert.h"
#include <exception>
#include <cstdio>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif

using std::string;

const string GameCtrl::MSG_BAD_ALLOC = "Oops! Not enough memory to run the game! Press any key to continue...";
const string GameCtrl::MSG_LOSE = "Sorry! You lose! Press any key to continue...";
const string GameCtrl::MSG_WIN = "Congratulations! You Win! Press any key to continue...";
const string GameCtrl::MSG_ESC = "Game ended! Press any key to continue...";
const string GameCtrl::MAP_INFO_FILENAME = "movements.txt";
const SearchableGrid::value_type GameCtrl::INF = 2147483647;

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
        Console::clear();
        init();
        startThreads();

        // Main thread waits
        while (1) {

        }

        return 0;
    } catch (const std::exception &e) {
        exitGameWithError(e.what());
        return -1;
    }
}

void GameCtrl::init() {
    initMap();
    initSnakes();
    if (writeToFile) {
        initFiles();
    }
}

void GameCtrl::initMap() {
    // Check validity
    if (mapRowCnt < 4 || mapColCnt < 4) {
        string msg = "Map size is at least 4*4. Current is "
            + Convert::toString(mapRowCnt) + "*" + Convert::toString(mapColCnt);
        throw std::range_error(msg.c_str());
    }
    
    // Initialize
    map = std::make_shared<Map>(mapRowCnt, mapColCnt);
    if (!map) {
        exitGame(MSG_BAD_ALLOC);
    } else {
        // Add some extra walls
    }
}

void GameCtrl::initSnakes() {
    if (runTest) {
        return;
    }

    snake1.setHeadType(Grid::GridType::SNAKEHEAD1);
    snake1.setBodyType(Grid::GridType::SNAKEBODY1);
    snake1.setTailType(Grid::GridType::SNAKETAIL1);
    snake1.setMap(map);
    snake1.addBody(Point(1, 3));
    snake1.addBody(Point(1, 2));
    snake1.addBody(Point(1, 1));

    if (enableSecondSnake) {
        snake2.setHeadType(Grid::GridType::SNAKEHEAD2);
        snake2.setBodyType(Grid::GridType::SNAKEBODY2);
        snake2.setTailType(Grid::GridType::SNAKETAIL2);
        snake2.setMap(map);
        snake2.addBody(Point(2, 3));
        snake2.addBody(Point(2, 2));
        snake2.addBody(Point(2, 1));
    }
}

void GameCtrl::initFiles() {
    movementFile = fopen(MAP_INFO_FILENAME.c_str(), "w");
    if (!movementFile) {
        throw std::runtime_error("Fail to open file: " + MAP_INFO_FILENAME);
    } else {
        // Write content description to the file
        string str = "Content description:\n";
        str += "#: wall\nH: snake head\nB: snake body\nT: snake tail\nF: food\n\n";
        str += "Movements:\n\n";
        fwrite(str.c_str(), sizeof(char), str.length(), movementFile);
    }
}

void GameCtrl::exitGame(const std::string &msg) {
    mutexExit.lock();

    // Stop threads
    sleepFor(100);
    stopThreads();
    sleepFor(100);

    // Close movement file if exists
    if (movementFile) {
        fclose(movementFile);
        movementFile = nullptr;
    }

    // Print message
    Console::setCursor(0, mapRowCnt + 9);
    Console::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::getch();

    mutexExit.unlock();
    exit(0);
}

void GameCtrl::exitGameWithError(const std::string &msg) {
    exitGame("Error: " + msg + "\nPress any key to continue...");
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
        try {
            s.move();
            if (writeToFile) {
                writeMapToFile();
            }
            mutexMove.unlock();
        } catch (const std::exception &e) {
            mutexMove.unlock();
            throw;  // Rethrow the exception
        }
    }
}

void GameCtrl::sleepFor(const long ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void GameCtrl::sleepByFPS() const {
    sleepFor(static_cast<long>((1.0 / fps) * 1000));
}

void GameCtrl::draw() {
    try {
        while (threadWork) {
            drawMapContent();
            drawGameInfo();
            sleepByFPS();
        }
    } catch (const std::exception &e) {
        exitGameWithError(e.what());
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
                case Grid::GridType::SNAKEBODY1:
                    Console::writeWithColor("  ", ConsoleColor(GREEN, GREEN, true, true));
                    break;
                case Grid::GridType::SNAKETAIL1:
                    Console::writeWithColor("  ", ConsoleColor(BLUE, BLUE, true, true));
                    break;
                case Grid::GridType::SNAKEHEAD2:
                    Console::writeWithColor("  ", ConsoleColor(MAGENTA, MAGENTA, true, true));
                    break;
                case Grid::GridType::SNAKEBODY2:
                    Console::writeWithColor("  ", ConsoleColor(CYAN, CYAN, true, true));
                    break;
                case Grid::GridType::SNAKETAIL2:
                    Console::writeWithColor("  ", ConsoleColor(BLUE, BLUE, true, true));
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
    Console::writeWithColor(" Space:  Toggle auto moving snake\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::writeWithColor("   Esc:  Exit game\n", ConsoleColor(WHITE, BLACK, true, false));
    Console::writeWithColor("\nScore: " + Convert::toString(snake1.size() + snake2.size()) + "\n",
                            ConsoleColor(WHITE, BLACK, true, false));
}

void GameCtrl::keyboard() {
    try {
        while (threadWork) {
            if (Console::kbhit()) {  // When keyboard is hit
                switch (Console::getch()) {
                    case 'w':
                        keyboardMove(snake1, Direction::UP);
                        break;
                    case 'a':
                        keyboardMove(snake1, Direction::LEFT);
                        break;
                    case 's':
                        keyboardMove(snake1, Direction::DOWN);
                        break;
                    case 'd':
                        keyboardMove(snake1, Direction::RIGHT);
                        break;
                    case 'i':
                        keyboardMove(snake2, Direction::UP);
                        break;
                    case 'j':
                        keyboardMove(snake2, Direction::LEFT);
                        break;
                    case 'k':
                        keyboardMove(snake2, Direction::DOWN);
                        break;
                    case 'l':
                        keyboardMove(snake2, Direction::RIGHT);
                        break;
                    case ' ':
                        toggleAutoMove();
                        break;
                    case 27:  // Esc
                        exitGame(MSG_ESC);
                        break;
                    default:
                        break;
                }
            }
            sleepByFPS();
        }
    } catch (const std::exception &e) {
        exitGameWithError(e.what());
    }
}

void GameCtrl::toggleAutoMove() {
    pauseMove = !pauseMove;
}

void GameCtrl::keyboardMove(Snake &s, const Direction &d) {
    if (autoMoveSnake && s.getDirection() == d) {
        moveSnake(s);  // Accelerate the movements
    }
    s.setDirection(d);
    if (!autoMoveSnake) {
        moveSnake(s);
    }
}

void GameCtrl::createFood() {
    try {
        while (threadWork) {
            if (!map->hasFood()) {
                map->createFood();
            }
            sleepByFPS();
        }
    } catch (const std::exception &e) {
        exitGameWithError(e.what());
    }
}

void GameCtrl::autoMove() {
    try {
        while (threadWork) {
            if (!pauseMove) {

                if (enableAI) {
                    snake1.decideNextDirection();
                }
                moveSnake(snake1);

                if (enableAI && enableSecondSnake) {
                    snake2.decideNextDirection();
                }

                if (enableSecondSnake) {
                    moveSnake(snake2);
                }

            }
            sleepFor(autoMoveInterval);
        }
    } catch (const std::exception &e) {
        exitGameWithError(e.what());
    }
}

void GameCtrl::startThreads() {
    // Detach each thread make each 
    // thread don't need to be joined
    drawThread = std::thread(&GameCtrl::draw, this);
    drawThread.detach();

    keyboardThread = std::thread(&GameCtrl::keyboard, this);
    keyboardThread.detach();

    if (runTest) {
        testThread = std::thread(&GameCtrl::test, this);
        testThread.detach();
    } else {
        foodThread = std::thread(&GameCtrl::createFood, this);
        foodThread.detach();

        if (autoMoveSnake) {
            autoMoveThread = std::thread(&GameCtrl::autoMove, this);
            autoMoveThread.detach();
        }
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

void GameCtrl::setRunTest(const bool &b) {
    runTest = b;
}

void GameCtrl::setMapRow(const Map::size_type &n) {
    mapRowCnt = n;
}

void GameCtrl::setMapColumn(const Map::size_type &n) {
    mapColCnt = n;
}

void GameCtrl::setEnableAI(const bool &enable) {
    enableAI = enable;
}

void GameCtrl::setWriteToFile(const bool &b) {
    writeToFile = b;
}

void GameCtrl::writeMapToFile() const {
    if (!movementFile) {
        return;
    }
    auto rows = map->getRowCount();
    auto cols = map->getColCount();
    for (Map::size_type i = 0; i < rows; ++i) {
        for (Map::size_type j = 0; j < cols; ++j) {
            switch (map->getGrid(Point(i, j)).getType()) {
                case Grid::GridType::EMPTY:
                    fwrite("  ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::WALL:
                    fwrite("# ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::FOOD:
                    fwrite("F ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::SNAKEHEAD1:
                    fwrite("H ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::SNAKEBODY1:
                    fwrite("B ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::SNAKETAIL1:
                    fwrite("T ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::SNAKEHEAD2:
                    fwrite("H ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::SNAKEBODY2:
                    fwrite("B ", sizeof(char), 2, movementFile);
                    break;
                case Grid::GridType::SNAKETAIL2:
                    fwrite("T ", sizeof(char), 2, movementFile);
                    break;
                default:
                    break;
            }
        }
        fwrite("\n", sizeof(char), 1, movementFile);
    }
    fwrite("\n", sizeof(char), 1, movementFile);
}

void GameCtrl::test() {
    try {
        // Check map size
        if (mapRowCnt < 20 || mapColCnt < 40) {
            throw std::range_error("Running testing program requires minimum map size 20*40.");
        }

        // Add walls for testing
        // Codes below become more effective when the map size is 20*40
        for (int i = 10; i < 30; ++i) {
            map->getGrid(Point(4, i)).setType(Grid::GridType::WALL);
            map->getGrid(Point(15, i)).setType(Grid::GridType::WALL);
        }
        for (int i = 4; i < 15; ++i) {
            map->getGrid(Point(i, 29)).setType(Grid::GridType::WALL);
        }

        // Test search algoritm
        Point from(9, 4), to(14, 31);
        std::list<Direction> path;
        map->setShowSearchDetails(true);
        map->findMinPath(from, to, path);
        //map->findMaxPath(from, to, path);
        std::string res = "Path from " + from.toString() + " to " + to.toString() + ": \n";
        for (const auto &d : path) {
            switch (d) {
                case LEFT:
                    res += "L ";
                    break;
                case UP:
                    res += "U ";
                    break;
                case RIGHT:
                    res += "R ";
                    break;
                case DOWN:
                    res += "D ";
                    break;
                case NONE:
                default:
                    res += "NONE ";
                    break;
            }
        }
        res += "\nPath length: " + Convert::toString(path.size());
        exitGame(res);
    } catch (const std::exception &e) {
        exitGameWithError(e.what());
    }
}
