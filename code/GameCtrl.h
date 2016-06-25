#pragma once

#include <string>

class GameCtrl {
public:
    static unsigned MAP_ROW;
    static unsigned MAP_COL;

    GameCtrl();
    ~GameCtrl();

    /*
    Start game.
    */
    int start();

    /*
    Exit the program with an exception message.

    @param msg the exception message
    */
    static void exitWithException(const std::string &msg);

private:

};
