#pragma once
#include "SDL2/SDL.h"

struct GameVariables {

        static int winWidth;
        static int winHeight;

        // Padding between the window edge and the wall against which the ball bounces (the court)
        const int initialPadding = 30;
        int currentPadding = initialPadding;
        
        const int paddlePadding = 20;

        // Paddle placement
        const int paddleW = 30;
        const int paddleH = 70;
        
        int paddleLX = currentPadding + 20;
        int paddleRX = winWidth - currentPadding - 20 - paddleW;
        int paddleY = (winHeight - paddleH) / 2;

        // Coordinates of the lines to render which defines the court
        SDL_Rect courtRect;
};