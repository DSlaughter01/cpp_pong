#pragma once
#include "SDL2/SDL.h"
#include "GameVariables.hpp"
#include <iostream>
#include <random>
#include <ctime>

class Paddle : public GameVariables {

    public:
        Paddle(char side);

        void Reset();
        void SetDirection(char direction);
        void Move();

    public:
        SDL_Rect rect;
    
    private:
        int dy;
        int x, y, w, h;
        const int paddleVel = 3;
};

class Ball : public GameVariables {
    public:
        SDL_Rect rect;
        float dx, dy;

    public:
        Ball();

        void Reset();
        void StartMovement();
        void Move();
        void ReverseDY();
        void CheckForPaddleCollision(Paddle &paddle);

    private:
        int x, y, w, h;
};
