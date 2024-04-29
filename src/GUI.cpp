#include "GUI.hpp"

int GameVariables::windowHeight = 600;
int GameVariables::windowWidth = 800;

int GameVariables::courtPadding = 30;
int GameVariables::paddlePadding = 30;

int GameVariables::courtLeftX;
int GameVariables::courtRightX;
int GameVariables::courtBottomY;
int GameVariables::courtTopY;

GUI::GUI() {
    // Initialise SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Problem initialising GUI: " << SDL_GetError() << std::endl;
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

    // Court edges
    courtLeftX = courtPadding;
    courtRightX = windowWidth - courtPadding;
    courtTopY = courtPadding;
    courtBottomY = windowHeight - courtPadding;
}

GUI::~GUI() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
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
    SDL_RenderPresent(renderer);
}