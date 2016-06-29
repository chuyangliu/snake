#include "GameCtrl.h"

int main() {
    auto game = GameCtrl::getInstance();

    // Default fps is 60.0
    game->setFPS(60.0);

    // Default is false
    game->setAutoMoveSnake(true);

    // Default is true
    game->setEnableKeyboard(true);

    // Default is 200 ms
    game->setAutoMoveInterval(200);

    return game->run();
}
