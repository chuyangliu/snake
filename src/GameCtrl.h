#pragma once

#include <string>
#include <thread>

class Map;

/*
Game controller class.
This is a singleton.
*/
class GameCtrl {
public:
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

private:
    Map *map = nullptr;
    std::thread *redrawThread = nullptr;

    /*
    Private constructor
    */
    GameCtrl();

    /*
    Draw the map content.
    */
    void draw();

    /*
    Start drawing the map content.
    */
    void startDrawing();

    /*
    Sleep the thread.

    @param time sleep time in ms.
    */
    void sleep_(const int time);
};
