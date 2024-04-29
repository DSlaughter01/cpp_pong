#include <iostream>
#include "Game.hpp"
#include "GUI.hpp"

int main(int argv, char** args) {

    GUI gui;
    Game game;

    game.GameLoop(gui);

    return 0;
}