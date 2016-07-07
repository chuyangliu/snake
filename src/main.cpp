#include "GameCtrl.h"

int main() {
    auto game = GameCtrl::getInstance();

    // Set map's size. Default is 20*20
    // The minimum size is 5*6 
    game->setMapRow(20);
    game->setMapColumn(20);

    // Set FPS. Default is 60.0
    game->setFPS(59.0);

    // Set whether to make the snake automove. Default is true
    game->setAutoMoveSnake(true);

    // Set interval time(ms) for automove. Default is 200.
    // If setAutoMoveSnake(false), this interval is useless.
    game->setAutoMoveInterval(200);

    // Set whether to enable the second snake. Default is false
    game->setEnableSecondSnake(true);

    // Set whether to run the test program. Default is false
    game->setRunTest(false);

    return game->run();
}
