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
    Exit the program with an exception message.

    @param msg the exception message
    */
    void exitWithException(const std::string &msg);

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
    void setMapRow(const int &n);

    /*
    Set map column count.
    */
    void setMapColumn(const int &n);

private:
    double fps = 60.0;
    bool autoMoveSnake = false;
    bool enableKeyboard = true;
    bool threadRun = true;
    long autoMoveInterval = 200;
    Snake *snake = nullptr;
    std::thread *drawThread = nullptr;
    std::thread *keyboardThread = nullptr;
    std::thread *foodThread = nullptr;
    std::thread *moveThread = nullptr;

    unsigned mapRowCnt = 20;
    unsigned mapColCnt = 20;

    /*
    Private constructor
    */
    GameCtrl();

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
    */
    void joinThreads();

    /*
    Stop all threads
    */
    void stopThreads();

    /*
    Release the memory occupied.
    */
    void release();
};
