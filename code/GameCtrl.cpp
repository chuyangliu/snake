#include "GameCtrl.h"
#include "Console.h"

unsigned GameCtrl::MAP_ROW = 12;
unsigned GameCtrl::MAP_COL = 12;

GameCtrl::GameCtrl() {
}

GameCtrl::~GameCtrl() {
}

int GameCtrl::start() {

    return 0;
}
 
void GameCtrl::exitWithException(const std::string &msg) {
    Console::writeWithColor(msg, FOREGROUND_RED | FOREGROUND_INTENSITY);
    exit(-1);
}
