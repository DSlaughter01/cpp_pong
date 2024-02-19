#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "GameVariables.hpp"
#include "GameObjects.hpp"

class GUI : public GameVariables {

    public:
        GUI();
        ~GUI();

        // Will get called every time the game resets
        void SetScore(int leftScore, int rightScore);

        // Get called every frame afterwards
        void RenderScore();
        void RenderCourt(Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball);
        void RenderScreen(Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball);

        void HandleWindowResize(SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle);

    private:
        static SDL_Window* window;
        static SDL_Renderer* renderer;

        SDL_Color textColour;
        TTF_Font* scoreFont;
        TTF_Font* startFont;
        
        SDL_Rect lScoreRect;
        SDL_Rect rScoreRect;
        SDL_Rect startTextRect;

        SDL_Texture* lScoreTxr;
        SDL_Texture* rScoreTxr;
        SDL_Texture* startTextTxr;

        const char* startText;
};