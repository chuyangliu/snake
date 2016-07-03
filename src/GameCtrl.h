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
    Set whether to enable keyboard instructions.
    */
    void setEnableKeyboard(const bool &enable);

    /*
    Set map row count.
    */
    void setMapRow(const Map::size_type &n);

    /*
    Set map column count.
    */
    void setMapColumn(const Map::size_type &n);

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
    bool autoMoveSnake = false;
    long autoMoveInterval = 200;
    bool enableKeyboard = true;

    // Game objects
    Snake *snake = nullptr;
    Map *map = nullptr;

    // Thread fields
    bool threadWork = true;      // Thread running switcher
    std::thread drawThread;      // Thread to draw the map
    std::thread keyboardThread;  // Thread to receive keyboard instructions
    std::thread foodThread;      // Thread to create food
    std::thread autoMoveThread;  // Thread to auto move the snake

    // Mutex variables
    std::mutex mutexMove;  // Mutex for snake movements
    std::mutex mutexExit;  // Mutex for exit game

    /*
    Private constructor for singleton.
    */
    GameCtrl();

    /*
    Release the memory occupied.
    */
    void release();

    /*
    Sleep for a time calculated by FPS value.
    */
    void sleepByFPS() const;

    /*
    Initialize the map of the game.
    */
    void initMap();

    /*
    Initialize the snake of the game.
    */
    void initSnake();

    /*
    Move snake and check game over.
    This is a thread-safe method.
    Call this method instead of snake->move() because
    snake->move() cannot check game over and is not thread-safe.
    */
    void moveSnake();

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
    Draw the map content.
    */
    void draw() const;

    /*
    Callback for keyboard thread.
    Execute keyboard instructions.
    */
    void keyboard();

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
};
