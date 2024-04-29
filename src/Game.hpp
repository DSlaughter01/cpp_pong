#pragma once
#include "GameVariables.hpp"
#include "GUI.hpp"
#include "GameObjects.hpp"
#include "SDL2/SDL.h"
#include <iostream>

class Game : public GameVariables {

    public:
        Game();
        ~Game();

        // Updates paddle velocity appropriately depending on key input from user, if the player is within the court bounds
        void HandleKeysInGame(const Uint8 * currentKeyboardState, SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle);

        void HandleCollisions(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        void MoveGameObjects(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        void HandleScore(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, char whoScored);

        void GameLoop(GUI gui);

    public:
        int leftScore, rightScore;

        int isInPlay;

    private:
        bool isRunning;

};