#pragma once

#include <string>
#include <thread>
#include "Snake.h"

/*
Game controller class.
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
    */
    int run();

    /*
    Print a message and exit the game.

    @param msg the message to be printed
    */
    void exitGame(const std::string &msg);

    /*
    Set the FPS.
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
    void setMapRow(const unsigned &n);

    /*
    Set map column count.
    */
    void setMapColumn(const unsigned &n);

private:
    bool autoMoveSnake = false;
    long autoMoveInterval = 200;
    bool enableKeyboard = true;
    bool threadWorking = true;  // True if all the threads are running

    Snake *snake = nullptr;
    std::thread *drawThread = nullptr;      // Thread to draw the map
    std::thread *keyboardThread = nullptr;  // Thread to receive keyboard instructions
    std::thread *foodThread = nullptr;      // Thread to create food
    std::thread *moveThread = nullptr;      // Thread to move the snake

    double fps = 60.0;
    unsigned mapRowCnt = 20;
    unsigned mapColCnt = 20;

    /*
    Private constructor
    */
    GameCtrl();

    /*
    Move snake and check game over
    Call this method instead of snake->move() in order to
    check game over after each movement.
    */
    void moveSnake();

    /*
    Create a snake.
    */
    void createSnake();

    /*
    Start drawing the map content.
    */
    void startDrawing();

    /*
    Called in redraw thread.
    Draw the map content.
    */
    void draw() const;

    /*
    Sleep the thread.
    This is a cross-platform method.

    @param ms sleep time in ms.
    */
    void sleepFor(const long ms) const;

    /*
    Sleep a while by the FPS.
    */
    void sleepByFPS() const;

    /*
    Start keyboard receiver thread.
    */
    void startKeyboardReceiver();

    /*
    Called in keyboard thread.
    Execute keyboard instructions.
    */
    void receiveKeyboardInstruction();

    /*
    Start creating food thread.
    */
    void startCreateFood();

    /*
    Called in food thread.
    */
    void createFood();

    /*
    Start auto moving the snake.
    */
    void startAutoMove();

    /*
    Called in move thread.
    */
    void autoMove();

    /*
    Start all threads.
    */
    void startThreads();

    /*
    Join all threads.
    Not used anymore
    */
    //void joinThreads();

    /*
    Stop all threads
    */
    void stopThreads();

    /*
    Release the memory occupied.
    */
    void release();
};
