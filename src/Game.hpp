#pragma once
#include "SDL2/SDL.h"
#include <iostream>
#include "GUI.hpp"
#include "GameVariables.hpp"
#include "GameObjects.hpp"
#include "SDL2/SDL_keyboard.h"

class Game : public GameVariables {

    public:
        Game();
        ~Game();

        void HandleStartEvents(SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball, GUI &gui);
        void HandleKeyboardEvents(SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball, GUI &gui);
        void HandleBallEvents(Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball, GUI &gui);
        void UpdateScore(Ball &ball);
        void GameLoop(GUI &gui);

        int leftScore;
        int rightScore;
        
    private:

        bool isRunning;
        bool isStartScreen;
        bool isInPlay;

        const int FPS = 60;
        const int FRAME_DELAY = 1000 / FPS; 
};