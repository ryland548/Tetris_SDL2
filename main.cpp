#include <iostream>
#include "./includes/game.hpp"

int main() {
    Game game = Game();
    game.load();
    game.run();
    return 0;
}

