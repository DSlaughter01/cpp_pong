#pragma once
#include "SDL2/SDL.h"
#include "GameVariables.hpp"

class Paddle : public GameVariables {

    public:
        int initX, initY;
        int yTop, yBot;
        float dy;
        SDL_Rect paddleRect;
        Paddle(int x, int y, int w, int h, int yTop, int yBot);
        void Reset();
        void MoveDown();
        void MoveUp();

};

class Ball : public GameVariables {

    public:
        int initX, initY;
        int ballDim;
        float dx, dy;
        int yTop, yBot, winWidth;

        Ball(int yTop, int yBot, int winWidth);
        void Reset();
        void Move();
        void HandleCollision(Paddle &leftPaddle, Paddle &rightPaddle);
        int IsOut();
        
        SDL_Rect ballRect;
};