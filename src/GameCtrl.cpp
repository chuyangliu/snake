#include "GameCtrl.h"
#include "util/util.h"
#include <stdexcept>
#include <cstdio>
#include <chrono>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#endif

using std::string;
using std::list;

// Error statements
const string GameCtrl::MSG_BAD_ALLOC = "Not enough memory to run the game.";
const string GameCtrl::MSG_LOSE = "Oops! You lose!";
const string GameCtrl::MSG_WIN = "Congratulations! You Win!";
const string GameCtrl::MSG_ESC = "Game ended.";
const string GameCtrl::MAP_INFO_FILENAME = "movements.txt";
int select_path=0; //Select Min Path or Max Path

GameCtrl::GameCtrl() {}

// class Destroyer
GameCtrl::~GameCtrl() {
    delete map;
    map = nullptr;
    if (movementFile) {
        fclose(movementFile);
        movementFile = nullptr;
    }
}

// Return the class instance's address
// Return instance will be used for creating and running game in main function
GameCtrl* GameCtrl::getInstance() {
=======
GameCtrl* GameCtrl::getInstance(int n) {

    static GameCtrl instance;
	select_path=n;
    return &instance;
}

void GameCtrl::setFPS(const double fps_) {
    fps = fps_;
}


// set program enable AI based search to decide next point to visit
// enableAI_(parameter) is boolean type whether the program use AI based search or not
void GameCtrl::setEnableAI(const bool enableAI_) {
    enableAI = enableAI_;
}


// set program enable Hamilton search to decide next point to visit
// enableHamilton_(parameter) is boolean type whether the program use Hamilton based search or not
void GameCtrl::setEnableHamilton(const bool enableHamilton_) {
    enableHamilton = enableHamilton_;
}

void GameCtrl::setMoveInterval(const long ms) {
    moveInterval = ms;
}

// set program records the snake's movement in text file
// b(parameter) is boolean type whether the program record snake's movement or not
void GameCtrl::setRecordMovements(const bool b) {
    recordMovements = b;
}

void GameCtrl::setRunTest(const bool b) {
    runTest = b;
}

void GameCtrl::setMapRow(const SizeType n) {
    mapRowCnt = n;
}

void GameCtrl::setMapCol(const SizeType n) {
    mapColCnt = n;
}

// essential function to working the program. this function call init() to initialize some variables to start game
// return 0 : successfully exit game
// return 1 : error is occured during game is running
int GameCtrl::run() {
    try {
        init();
        if (runTest) {
            test();
        }
        while (runMainThread) {}
        return 0;
    } catch (const std::exception &e) {
        exitGameErr(e.what());
        return -1;
    }
}

void GameCtrl::sleepFPS() const {
    util::sleep((long)((1.0 / fps) * 1000));
}

// this function is called when the program is needed to end
// msg(parameter) contains error message
// this function use mutex lock because when a thread access critical section, other two thread must not access same crtical section.
// ex) when draw thread is errored, and at the same time , search thread is also errored,    
//     the first errored thread access critical section, and then search thread access critical section
void GameCtrl::exitGame(const std::string &msg) {
    mutexExit.lock();
    if (runMainThread) {
        util::sleep(100);
        runSubThread = false;
        util::sleep(100);
        printMsg(msg);
    }
    mutexExit.unlock();
    runMainThread = false;
}

void GameCtrl::exitGameErr(const std::string &err) {
    exitGame("ERROR: " + err);
}

void GameCtrl::printMsg(const std::string &msg) {
    Console::setCursor(0, (int)mapRowCnt);
    Console::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
}

// this function is for moving snake one step more closer to food
// this function also use mutex lock because manually moving mode, 
// user input many keys, but the process must execute move(), writeMapToFile(), CreateRandFood() one by one
void GameCtrl::moveSnake() {
    mutexMove.lock();
    if (map->isAllBody()) {
        mutexMove.unlock();
        exitGame(MSG_WIN);
    } else if (snake.isDead()) {
        mutexMove.unlock();
        exitGame(MSG_LOSE);
    } else {
        try {
            snake.move();
            if (recordMovements && snake.getDirection() != NONE) {
                writeMapToFile();
            }
            if (!map->hasFood()) {
                map->createRandFood();
            }
            mutexMove.unlock();
        } catch (const std::exception) {
            mutexMove.unlock();
            throw;
        }
    }
}

// this function is for writing snake's movement to text file format
void GameCtrl::writeMapToFile() const {
    if (!movementFile) {
        return;
    }
    SizeType rows = map->getRowCount();
    SizeType cols = map->getColCount();
    for (SizeType i = 0; i < rows; ++i) {
        for (SizeType j = 0; j < cols; ++j) {
            switch (map->getPoint(Pos(i, j)).getType()) {
                case Point::Type::EMPTY:
                    fwrite("  ", sizeof(char), 2, movementFile); break;
                case Point::Type::WALL:
                    fwrite("# ", sizeof(char), 2, movementFile); break;
                case Point::Type::FOOD:
                    fwrite("F ", sizeof(char), 2, movementFile); break;
                case Point::Type::SNAKE_HEAD:
                    fwrite("H ", sizeof(char), 2, movementFile); break;
                case Point::Type::SNAKE_BODY:
                    fwrite("B ", sizeof(char), 2, movementFile); break;
                case Point::Type::SNAKE_TAIL:
                    fwrite("T ", sizeof(char), 2, movementFile); break;
                default:
                    break;
            }
        }
        fwrite("\n", sizeof(char), 1, movementFile);
    }
    fwrite("\n", sizeof(char), 1, movementFile);
}

// this function is calling functions to initialize map and snake, and optionally calling initFiles()
void GameCtrl::init() {
    Console::clear();
    initMap();
    if (!runTest) {
        initSnake();
        if (recordMovements) {
            initFiles();
        }
    }
    startThreads();
}

// this function is for initialize map
void GameCtrl::initMap() {
    if (mapRowCnt < 5 || mapColCnt < 5) {
        string msg = "GameCtrl.initMap(): Map size at least 5*5. Current size "
            + util::toString(mapRowCnt) + "*" + util::toString(mapColCnt) + ".";
        throw std::range_error(msg.c_str());
    }
    map = new Map(mapRowCnt, mapColCnt);
    if (!map) {
        exitGameErr(MSG_BAD_ALLOC);
    } else {
        // Add some extra walls manully
    }
}

// this function is for initialize snake and optionally call snake.hamilton search
void GameCtrl::initSnake() {
    snake.setMap(map);
    snake.addBody(Pos(1, 3));
    snake.addBody(Pos(1, 2));
    snake.addBody(Pos(1, 1));
    if (enableHamilton) {
        snake.enableHamilton();
    }
}

// this function is for initialize movement file to record snake's movement
// initializing file's main work is write content type description to file
void GameCtrl::initFiles() {
    movementFile = fopen(MAP_INFO_FILENAME.c_str(), "w");
    if (!movementFile) {
        throw std::runtime_error("GameCtrl.initFiles(): Fail to open file: " + MAP_INFO_FILENAME);
    } else {
        // Write content description to the file
        string str = "Content description:\n";
        str += "#: wall\nH: snake head\nB: snake body\nT: snake tail\nF: food\n\n";
        str += "Movements:\n\n";
        fwrite(str.c_str(), sizeof(char), str.length(), movementFile);
    }
}

// this function creates three thrads which are necessary to run the game
// the three thread is drawThread, keyboardThread, moveThread
// drawThread's main work is draw map in console
// keyboardThread's main work is inputing key and calling function to move snake position
// moveThread's main work is to move snake position based on auto search methods(Hamilton, graph search, BFS)
// runSubThread is very important thing to run program. as long as this value is true, each thread is keep running and it means no error is occured.
void GameCtrl::startThreads() {
    runSubThread = true;
    drawThread = std::thread(&GameCtrl::draw, this);
    drawThread.detach();
    keyboardThread = std::thread(&GameCtrl::keyboard, this);
    keyboardThread.detach();
    if (!runTest) {
        moveThread = std::thread(&GameCtrl::autoMove, this);
        moveThread.detach();
    }
}
// this function calls drawMapContent() function to draw Map
void GameCtrl::draw() {
    try {
        while (runSubThread) {
            drawMapContent();
            sleepFPS();
        }
    } catch (const std::exception &e) {
        exitGameErr(e.what());
    }
}
// this function draws Map in console according to point type
// setCursor needs x,y parameter, but when parameter is no, the default is x,y = 0 and this function just position console's cursor to write map
void GameCtrl::drawMapContent() const {
    Console::setCursor();
    SizeType row = map->getRowCount();
	SizeType col = map->getColCount();
    for (SizeType i = 0; i < row; ++i) {
        for (SizeType j = 0; j < col; ++j) {
            const Point &point = map->getPoint(Pos(i, j));
            switch (point.getType()) {
                case Point::Type::EMPTY:
                    Console::writeWithColor("  ", ConsoleColor(BLACK, BLACK));
                    break;
                case Point::Type::WALL:
                    Console::writeWithColor("  ", ConsoleColor(WHITE, WHITE, true, true));
                    break;
                case Point::Type::FOOD:
                    Console::writeWithColor("  ", ConsoleColor(YELLOW, YELLOW, true, true));
                    break;
                case Point::Type::SNAKE_HEAD:
                    Console::writeWithColor("  ", ConsoleColor(RED, RED, true, true));
                    break;
                case Point::Type::SNAKE_BODY:
                    Console::writeWithColor("  ", ConsoleColor(GREEN, GREEN, true, true));
                    break;
                case Point::Type::SNAKE_TAIL:
                    Console::writeWithColor("  ", ConsoleColor(BLUE, BLUE, true, true));
                    break;
                case Point::Type::TEST_VISIT:
                    drawTestPoint(point, ConsoleColor(BLUE, GREEN, true, true));
                    break;
                case Point::Type::TEST_PATH:
                    drawTestPoint(point, ConsoleColor(BLUE, RED, true, true));
                    break;
                default:
                    break;
            }
        }
        Console::write("\n");
    }
}

// this function is for drawing point in test mode
// after debugging, I notice that the code never enter the MAX_VALUE
void GameCtrl::drawTestPoint(const Point &p, const ConsoleColor &consoleColor) const {
    string pointStr = "";
    if (p.getDist() == Point::MAX_VALUE) {
        pointStr = "In";
    } else if (p.getDist() == EMPTY_VALUE) {
        pointStr = "  ";
    } else {
        Point::ValueType dist = p.getDist();
        pointStr = util::toString(p.getDist());
        if (dist / 10 == 0) {
            pointStr.insert(0, " ");
        } 
    }
    Console::writeWithColor(pointStr, consoleColor);
}

// this function is call keyboardMove function according to key
void GameCtrl::keyboard() {
    try {
        while (runSubThread) {
            if (Console::kbhit()) {
                switch (Console::getch()) {
					case 'w': case 'W':
                        keyboardMove(snake, Direction::UP);
                        break;
					case 'a': case 'A':
                        keyboardMove(snake, Direction::LEFT);
                        break;
					case 's': case 'S':
                        keyboardMove(snake, Direction::DOWN);
                        break;
					case 'd': case 'D':
                        keyboardMove(snake, Direction::RIGHT);
                        break;
                    case ' ':
                        pause = !pause;  // Pause or resume game
                        break;
                    case 27:  // Esc
                        exitGame(MSG_ESC);
                        break;
                    default:
                        break;
                }
            }
            sleepFPS();
        }
    } catch (const std::exception &e) {
        exitGameErr(e.what());
    }
}

// this function is for moving snake 1 step toward manual input key
// when pause mode, snake moves toward manual input key or
// when not pause mode and when BFS mode(not AI search mode), move snake toward input key
void GameCtrl::keyboardMove(Snake &s, const Direction d) {
    if (pause) {
        s.setDirection(d);
        moveSnake();
    } else if (!enableAI) {
        if (s.getDirection() == d) {
            moveSnake();  // Accelerate
        } else {
            s.setDirection(d);
        }
    }
}

// this function is for auto moving snake 1 step toward AI based search path
// snake.cpp's decideNext function is only for AI based search(hamilton, graph)
// when enableAI == False, it determine path using BFS search method. 
void GameCtrl::autoMove() {
    try {
        while (runSubThread) {
            util::sleep(moveInterval);
            if (!pause) {
                if (enableAI) {
                    snake.decideNext();
                }
                moveSnake();
            }
        }
    } catch (const std::exception &e) {
        exitGameErr(e.what());
    }
}

// this function is for testing several tests such as Food creating test, searching minimum/maximum path test, search using Hamilton test
void GameCtrl::test() {
    //testFood();
    testSearch();
    //testHamilton();
}

// this function is for creating food randomly test
void GameCtrl::testFood() {
    SizeType cnt = 0;
    while (runMainThread && cnt++ < map->getSize()) {
        map->createRandFood();
        sleepFPS();
    }
    exitGame("testFood() finished.");
}

// this function is for searching minimum/maximum path test calling snake.testMinPath
// you can choose BFS method with minimum path search or search maximum path using BFS method and extra extension algorithm.
void GameCtrl::testSearch() {
    if (mapRowCnt != 20 || mapColCnt != 20) {
        throw std::range_error("GameCtrl.testSearch(): Require map size 20*20.");
    }

    list<Direction> path;
    snake.setMap(map);

    // Add walls for testing
    for (int i = 4; i < 16; ++i) {
        map->getPoint(Pos(i, 9)).setType(Point::Type::WALL);   // vertical
        map->getPoint(Pos(4, i)).setType(Point::Type::WALL);   // horizontal #1
        map->getPoint(Pos(15, i)).setType(Point::Type::WALL);  // horizontal #2
    }
   
    Pos from(6, 7);
	Pos to(14, 13);
	if(select_path==0) snake.testMinPath(from, to, path);
    else if(select_path==1) snake.testMaxPath(from, to, path);

    // Print path info
    string info = "Path from " + from.toString() + " to " + to.toString()
        + " of length " + util::toString(path.size()) + ":\n";
    for (const Direction &d : path) {
        switch (d) {
            case LEFT:
                info += "L "; break;
            case UP:
                info += "U "; break;
            case RIGHT:
                info += "R "; break;
            case DOWN:
                info += "D "; break;
            case NONE:
            default:
                info += "NONE "; break;
        }
    }
    info += "\ntestSearch() finished.";
    exitGame(info);
}

// this function is for testing hamilton path search
void GameCtrl::testHamilton() {
    snake.setMap(map);
    snake.addBody(Pos(1, 3));
    snake.addBody(Pos(1, 2));
    snake.addBody(Pos(1, 1));
    snake.testHamilton();
    exitGame("testHamilton() finished.");
}
