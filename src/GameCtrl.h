#pragma once

#include <string>
#include <thread>
#include <mutex>
#include "Snake.h"

/*
A game controller to control the game.
This is a singleton.
*/
class GameCtrl {
public:
    // Game exit messages
    static const std::string MSG_BAD_ALLOC;
    static const std::string MSG_LOSE;
    static const std::string MSG_WIN;
    static const std::string MSG_ESC;
    static const SearchableGrid::value_type INF;
    static const std::string MAP_INFO_FILENAME;

    ~GameCtrl();

    /*
    Forbid copy
    */
    GameCtrl(const GameCtrl &m) = delete;
    GameCtrl& operator=(const GameCtrl &m) = delete;

    /*
    Return the only instance
    */
    static GameCtrl* getInstance();

    /*
    Run the game.

    @return the exit status of the program.
            Use it as the return value of the main function.
    */
    int run();

    /*
    Print a message and exit the game.
    This is a thread-safe method.

    @param msg the message to be printed
    */
    void exitGame(const std::string &msg);

    /*
    Print a error message and exit the game.
    This is a thread-safe method.

    @param msg the error message
    */
    void exitGameWithError(const std::string &msg);

    /*
    Set fps value.
    */
    void setFPS(const double &fps_);

    /*
    Set whether to auto move the snake.
    */
    void setAutoMoveSnake(const bool &move);

    /*
    Set the interval between snake's auto movements.

    @param ms the interval in milliseconds.
    */
    void setAutoMoveInterval(const long &ms);

    /*
    Set whether to enable the second snake.
    */
    void setEnableSecondSnake(const bool &enable);

    /*
    Set whether to enable snake AI.
    */
    void setEnableAI(const bool &enable);

    /*
    Set whether to run the test thread.
    */
    void setRunTest(const bool &b);

    /*
    Set map row count.
    */
    void setMapRow(const Map::size_type &n);

    /*
    Set map column count.
    */
    void setMapColumn(const Map::size_type &n);

    /*
    Set whether to write the map content to the file
    */
    void setWriteToFile(const bool &b);

    /*
    Return a random number in [min, max]
    */
    int random(const int min, const int max);

    /*
    Sleep current thread.
    This is a cross-platform method.

    @param ms sleep time in ms.
    */
    void sleepFor(const long ms) const;

private:
    // Game conf fields
    double fps = 60.0;
    Map::size_type mapRowCnt = 20;
    Map::size_type mapColCnt = 20;
    bool autoMoveSnake = true;
    long autoMoveInterval = 200;
    bool enableSecondSnake = false;
    bool enableAI = false;
    bool runTest = false;
    bool writeToFile = false;

    bool pauseMove = false;

    // Game objects
    Snake snake1;
    Snake snake2;
    std::shared_ptr<Map> map;

    // Thread fields
    bool threadWork = true;      // Thread running switcher
    std::thread drawThread;      // Thread to draw the map
    std::thread keyboardThread;  // Thread to receive keyboard instructions
    std::thread foodThread;      // Thread to create food
    std::thread autoMoveThread;  // Thread to auto move the snake
    std::thread testThread;      // Thread to test different features.

    // Mutex variables
    std::mutex mutexMove;  // Mutex for moveSnake()
    std::mutex mutexExit;  // Mutex for exitGame()

    // File pointer
    FILE *movementFile = nullptr;  // File to save snake movements

    /*
    Private constructor for singleton.
    */
    GameCtrl();

    /*
    Sleep for a time calculated by FPS value.
    */
    void sleepByFPS() const;

    /*
    Initialize game configuration.
    */
    void init();

    /*
    Initialize the map of the game.
    */
    void initMap();

    /*
    Add new walls on the map.
    */
    void addWalls();

    /*
    Initialize the snakes of the game.
    */
    void initSnakes();

    /*
    Move snake and check if the game is over.
    Notice this is a thread-safe method. Make sure to call 
    this method instead of snake.move() because snake.move()
    is not thread-safe.

    @param s the snake to move
    */
    void moveSnake(Snake &s);

    /*
    Start all threads.
    */
    void startThreads();

    /*
    Stop all threads
    */
    void stopThreads();

    /*
    Callback for draw thread.
    Draw the game elements.
    */
    void draw();

    /*
    Draw the map content.
    */
    void drawMapContent() const;

    /*
    Draw the game info.
    */
    void drawGameInfo() const;

    /*
    Callback for keyboard thread.
    Execute keyboard instructions.
    */
    void keyboard();

    /*
    Execute keyboard move instruction.

    @param s the snake to move
    @param d the direction to move
    */
    void keyboardMove(Snake &s, const Direction &d);

    /*
    Toggle the auto move thread.
    */
    void toggleAutoMove();

    /*
    Callback for food thread.
    Create food on the map if it doesn't exist.
    */
    void createFood();

    /*
    Callback for auto move thread.
    Auto move the snake on the map.
    */
    void autoMove();

    /*
    Call back for test thread.
    Test different features
    */
    void test();

    /*
    Write the map content to movement file.
    */
    void writeMapToFile() const;
};
