#include <iostream>

#include "Game/IGame.h"
#include "IO/CommandHandler.h"

int main() {
    IGame* game = nullptr;
    CommandHandler handler(game);
    handler.Run();
    if (game) {
        delete game;
    }
    return 0;
}
