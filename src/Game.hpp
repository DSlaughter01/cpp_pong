#pragma once
#include "GameVariables.hpp"
#include "GUI.hpp"
#include "GameObjects.hpp"
#include "SDL2/SDL.h"
#include <iostream>

class Game : public GameVariables {

    public:
        int leftScore, rightScore;

    private:
        bool isRunning;
        bool resize;

    public:
        Game();
        ~Game();

        // Updates paddle velocity appropriately depending on key input from user, if the player is within the court bounds
        void HandleKeysInGame(const Uint8 * currentKeyboardState, SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle);

        // Handles collision between the ball and the paddles
        void HandleCollisions(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        // Moves the ball and both of the paddles according to their velocity
        void MoveGameObjects(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        // Resets object position and velocity, and updates the score. whoScored should be 'l' (left) or 'r' (right)
        void HandleScore(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, char whoScored, GUI &gui);

        // Changes variables, repositions objects, and updates GUI after a window resize event
        void HandleWindowResize(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, GUI &gui);

        void HandleGameOver(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, char winner, GUI &gui);

        void ResetToNewGame(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, GUI &gui);

        void GameLoop(GUI gui);
};