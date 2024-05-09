#pragma once
#include "SDL2/SDL.h"

struct GameVariables {

    // Initialised by the GUI
    static int windowWidth, windowHeight;

    static int courtPadding, paddlePadding;
    static int textPadding;

    static int courtLeftX, courtRightX, courtTopY, courtBottomY;

    const int FPS = 60;
    const int desiredFrameTicks = 1000 / FPS;

    const int initialBallDX = 2;
    const int maxBallVel = 8;

    const SDL_Color textColour {230, 230, 230, 255};

    static bool isInPlay;

};