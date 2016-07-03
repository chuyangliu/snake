#include "GameCtrl.h"

int main() {
    auto game = GameCtrl::getInstance();

    // Set map's size. Default is 20*20
    // The minimum size is 3*6 because there must be
    // enough space to hold three snake bodies and
    // one food.
    game->setMapRow(20);
    game->setMapColumn(20);

    // Set FPS. Default is 60.0
    game->setFPS(59.0);

    // Set whether to make the snake automove. Default is false
    game->setAutoMoveSnake(true);

    // Set interval time for automove. Default is 200.
    // If setAutoMoveSnake(false), this interval is useless.
    game->setAutoMoveInterval(200);

    // Set whether to use keyboard instructions. Default is true
    game->setEnableKeyboard(true);

    return game->run();
}

/*
TODO
1. GameCtrl stores snake list
2. AI
*/
