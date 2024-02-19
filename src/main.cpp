#include <iostream>
#include "Game.hpp"
#include "GUI.hpp"

int main(int argv, char** args) {

    GUI gui;
    Game game;

    game.GameLoop(gui);

    // Destructors take care of clearup
    
    return 0;
}