#pragma once

#include <iostream>
#include "GameVariables.hpp"
#include "GameObjects.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string.h>
#include <sstream>

class GUI : public GameVariables {

    public:
        GUI();
        ~GUI();

        void GenerateSpaceText();

        // Updates score textures to accommodate different scores 
        void UpdateScoreTex(int leftScore, int rightScore);

        // Draws court lines and game objects.
        void DrawCourt(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        // Renders the screen to renderer.
        void RenderScreen(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle);

        void GenerateGameOverScreen(char winner);

        // Sets the variables for window height and width, and returns {widthDifference, heightDifference}
        std::pair<int, int> ChangeWindowSize();

    
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::string leftPlayer, rightPlayer;
        std::string leftScoreStr, rightScoreStr, pressSpaceStr, gameOverStr;

        SDL_Surface* leftScoreSurf;
        SDL_Surface* rightScoreSurf;
        SDL_Surface* pressSpaceSurf;
        SDL_Surface* gameOverSurf;

        SDL_Texture* leftScoreTex;
        SDL_Texture* rightScoreTex;
        SDL_Texture* pressSpaceTex;
        SDL_Texture* gameOverTex;

        SDL_Rect leftScoreDest;
        SDL_Rect rightScoreDest;
        SDL_Rect pressSpaceDest;
        SDL_Rect gameOverDest;

        TTF_Font* gameFont;
};