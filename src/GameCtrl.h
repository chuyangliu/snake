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
    // Constants
    static unsigned MAP_ROW;
    static unsigned MAP_COL;

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
    Start game.
    */
    int start();

    /*
    Exit the program with an exception message.

    @param msg the exception message
    */
    void exitWithException(const std::string &msg);

    /*
    Set the FPS.
    Default is 60.
    */
    void setFPS(const double &fps_);

private:
    double fps = 1.0 / 60.0;
    bool threadRun = true;
    Snake *snake = nullptr;
    std::thread *drawThread = nullptr;
    std::thread *keyboardThread = nullptr;
    std::thread *foodThread = nullptr;

    /*
    Private constructor
    */
    GameCtrl();

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

    @param time sleep time in ms.
    */
    void sleep_(const long time) const;

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
