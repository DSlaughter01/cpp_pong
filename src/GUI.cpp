#include "GUI.hpp"

// Static variables
int GameVariables::windowHeight = 600;
int GameVariables::windowWidth = 800;

int GameVariables::courtPadding = 30;
int GameVariables::paddlePadding = 30;
int GameVariables::textPadding = 15;

int GameVariables::courtLeftX;
int GameVariables::courtRightX;
int GameVariables::courtBottomY;
int GameVariables::courtTopY;

GUI::GUI() :
    leftScoreSurf(nullptr), rightScoreSurf(nullptr), leftScoreTex(nullptr), rightScoreTex(nullptr)
{

    // Initialise SDL and TTF
    SDL_Init(SDL_INIT_EVERYTHING);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Problem initialising GUI: " << SDL_GetError() << std::endl;
        return;
    }

    TTF_Init();
    if (TTF_Init() < 0) {
        std::cerr << "Problem initialising font library" << TTF_GetError() << std::endl;
        return;
    }

    // Create a resizeable window
    window = SDL_CreateWindow("Pong", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              windowWidth, 
                              windowHeight, 
                              SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be initialised: " << SDL_GetError() << std::endl;
        return;
    }

    // Create a renderer, and set its draw colour to dark grey
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Renderer could not be initialised: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);

    // Define the court limits
    courtLeftX = courtPadding;
    courtRightX = windowWidth - courtPadding;
    courtTopY = courtPadding;
    courtBottomY = windowHeight - courtPadding;

    // Score display
    gameFont = TTF_OpenFont("src/Roboto-Regular.ttf", 32);
    pressSpaceStr = "Press space to start";
    GenerateSpaceText();
    UpdateScoreTex(0, 0);
}


GUI::~GUI() {

    TTF_CloseFont(gameFont);

    TTF_Quit();
    SDL_Quit();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_FreeSurface(leftScoreSurf);
    SDL_FreeSurface(rightScoreSurf);
    SDL_FreeSurface(pressSpaceSurf);

    SDL_DestroyTexture(leftScoreTex);
    SDL_DestroyTexture(rightScoreTex);
    SDL_DestroyTexture(pressSpaceTex);
}


void GUI::GenerateSpaceText() {
    pressSpaceSurf = TTF_RenderText_Solid(gameFont, pressSpaceStr.c_str(), textColour);
    pressSpaceTex = SDL_CreateTextureFromSurface(renderer, pressSpaceSurf);
    pressSpaceDest = {(windowWidth - pressSpaceSurf->w) / 2,
                       3 * windowHeight / 4 - pressSpaceSurf->h / 2,
                       pressSpaceSurf->w,
                       pressSpaceSurf->h};
}


void GUI::UpdateScoreTex(int leftScore, int rightScore) {

    // Free memory
    if (leftScoreSurf)
        SDL_FreeSurface(leftScoreSurf);
    if (rightScoreSurf)
        SDL_FreeSurface(rightScoreSurf);
    if (leftScoreTex)
        SDL_DestroyTexture(leftScoreTex);
    if (rightScoreTex)
        SDL_DestroyTexture(rightScoreTex);

    // Take as input the score
    leftScoreStr = std::to_string(leftScore);
    rightScoreStr = std::to_string(rightScore);

    // Create surface and texture
    leftScoreSurf = TTF_RenderText_Solid(gameFont, leftScoreStr.c_str(), textColour);
    rightScoreSurf = TTF_RenderText_Solid(gameFont, rightScoreStr.c_str(), textColour);

    leftScoreTex = SDL_CreateTextureFromSurface(renderer, leftScoreSurf);
    rightScoreTex = SDL_CreateTextureFromSurface(renderer, rightScoreSurf);

    // Define destination rectangles
    leftScoreDest = {windowWidth / 4 - leftScoreSurf->w / 2, 
                     courtPadding + textPadding, 
                     leftScoreSurf->w, 
                     leftScoreSurf->h};
    rightScoreDest = {3 * windowWidth / 4 - rightScoreSurf->w / 2, 
                      courtPadding + textPadding, 
                      rightScoreSurf->w, 
                      rightScoreSurf->h};
}


void GUI::DrawCourt(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle) {
    
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, SDL_ALPHA_OPAQUE);

    // Draw outside court lines
    SDL_RenderDrawLine(renderer, courtLeftX, courtTopY, courtRightX, courtTopY); // Top
    SDL_RenderDrawLine(renderer, courtLeftX, courtBottomY, courtRightX, courtBottomY); // Bottom
    SDL_RenderDrawLine(renderer, courtLeftX, courtTopY, courtLeftX, courtBottomY); // Left vert
    SDL_RenderDrawLine(renderer, courtRightX, courtTopY, courtRightX, courtBottomY); // Right vert
    SDL_RenderDrawLine(renderer, windowWidth / 2, courtPadding, windowWidth / 2, windowHeight - courtPadding); // Half way

    // Draw the ball and paddles
    SDL_RenderFillRect(renderer, &ball.rect);
    SDL_RenderFillRect(renderer, &leftPaddle.rect);
    SDL_RenderFillRect(renderer, &rightPaddle.rect);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
}


void GUI::RenderScreen(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle) {

    SDL_RenderClear(renderer);
    DrawCourt(ball, leftPaddle, rightPaddle);
    if (!isInPlay)
        SDL_RenderCopy(renderer, pressSpaceTex, NULL, &pressSpaceDest);
    SDL_RenderCopy(renderer, leftScoreTex, NULL, &leftScoreDest);
    SDL_RenderCopy(renderer, rightScoreTex, NULL, &rightScoreDest);
    SDL_RenderPresent(renderer);
}

std::pair<int, int> GUI::ChangeWindowSize() {

    int currWidth = windowWidth;
    int currHeight = windowHeight;

    // Change the window width and height variables
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Change score and space bar placement
    leftScoreDest.x = windowWidth / 4 - leftScoreSurf->w / 2;
    rightScoreDest.x = 3 * windowWidth / 4 - rightScoreSurf->w / 2;
    pressSpaceDest.x = (windowWidth - pressSpaceSurf->w) / 2;
    pressSpaceDest.y = 3 * windowHeight / 4 - pressSpaceSurf->h / 2;

    // Difference between the old and new dimensions
    std::pair<int, int> result = {windowWidth - currWidth, windowHeight - currHeight};
    
    return result;
}