#pragma once
#include "SDL2/SDL.h"
#include "GameVariables.hpp"
#include <iostream>
#include <random>
#include <ctime>

class Paddle : public GameVariables {

    public:
        SDL_Rect rect;
    
    private:
        char side;
        int initialX, initialY;
        int dy;
        int x, y, w, h;
        const int paddleVel = 3;

    public:
        // side should be 'l' (left) or 'r' (right).
        Paddle(char side);

        // Resets the paddle to its original rect position and velocity.
        void Reset();

        // Sets the paddle's dy according to direction 'u' (up), 'd' (down), or 'n' (none).
        void SetVelocity(char direction);

        // Updates the paddle's rect according to its dy (velocity).
        void Move();

        void ReactToWindowResize(int windowWidthChange, int windowHeightChange);
};

class Ball : public GameVariables {

    public:
        SDL_Rect rect;
        float dx, dy;

    private:
        int x, y, w, h;

    public:
        Ball();

        // Resets the ball to its original rect position and velocity.
        void Reset();

        // Sets dy to a random float between -2 and 2, and dx to GameVariables::initialBallDX.
        // Called when Game::isInPlay is changed from false to true.
        void StartMovement();

        // Updates the ball's rect according to its dx and dy.
        void Move();

        // Reverses dy. Called when the ball hits the upper or lower court boundaries.
        void ReverseDY();

        // Reverses dx and accelerates. Called under certain conditions on rect and dx.
        void BounceOffPaddle();

        void ReactToWindowResize(int windowWidthChange, int windowHeightChange);
};
