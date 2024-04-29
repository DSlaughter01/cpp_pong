#pragma once

#include <iostream>
#include "GameVariables.hpp"
#include "GameObjects.hpp"
#include "SDL2/SDL.h"

class GUI : public GameVariables {

    public:
        GUI();
        ~GUI();

        // Draws court lines and game objects.
        void DrawCourt(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        // Renders the screen to renderer
        void RenderScreen(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);
    
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
};