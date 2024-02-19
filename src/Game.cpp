#include "Game.hpp"

int GameVariables::winWidth = 800;
int GameVariables::winHeight = 600;

Game::Game()
    : isRunning(true), isStartScreen(true), isInPlay(false), leftScore(0), rightScore(0) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Uh-oh! Something's wrong! " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
}


Game::~Game() {
    SDL_Quit();
}

void Game::HandleStartEvents(SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball, GUI &gui) {

    // Press space to start
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        isStartScreen = false;
        isInPlay = true;
        isRunning = true;
        return;
    }

    // Resize the window
    else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        gui.HandleWindowResize(event, leftPaddle, rightPaddle);
        gui.RenderScreen(leftPaddle, rightPaddle, ball);
        return;
    }

    else if (event.type == SDL_QUIT) {
        isStartScreen = false;
        isRunning = false;
        return;
    }
}

void Game::HandleKeyboardEvents(SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball, GUI &gui) {

    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // Check for quit
    if (event.type == SDL_QUIT) {
        isRunning = false;
        return;
    }

    // Check for resize
    else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        gui.HandleWindowResize(event, leftPaddle, rightPaddle);
        return;
    }

    // Move paddles depending on keyboard state 
    if (keyboardState[SDL_SCANCODE_UP]) {
        rightPaddle.MoveUp();
    }
    else if (keyboardState[SDL_SCANCODE_DOWN]) {
        rightPaddle.MoveDown();
    }
    if (keyboardState[SDL_SCANCODE_W]) {
        leftPaddle.MoveUp();
    }
    else if (keyboardState[SDL_SCANCODE_S]) {
        leftPaddle.MoveDown();
    }
}

void Game::UpdateScore(Ball &ball) {

    if (ball.IsOut() == 1)
        leftScore++;

    else if (ball.IsOut() == -1)
        rightScore++;

    std::cout << leftScore << rightScore << std::endl;
}


void Game::HandleBallEvents(Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball, GUI &gui) {

    ball.Move();

    ball.HandleCollision(leftPaddle, rightPaddle);

    if (ball.IsOut() != 0) {

        // Reset position (and ball velocity)
        ball.Reset();
        leftPaddle.Reset();
        rightPaddle.Reset();

        // Update score in game and reflect on GUI
        UpdateScore(ball);
        gui.SetScore(leftScore, rightScore);

        // Go to start screen
        isStartScreen = true;
        isInPlay = false;
    }
}


void Game::GameLoop(GUI &gui) {

    int yTop = currentPadding;
    int yBot = winHeight - currentPadding;

    // Game objects, created here for stack memory rather than allocating new in constructor
    Paddle leftPaddle(paddleLX, paddleY, paddleW, paddleH, yTop, yBot);
    Paddle rightPaddle(paddleRX, paddleY, paddleW, paddleH, yTop, yBot);
    Ball ball(yTop, yBot, winWidth);

    SDL_Event event;

    while (isRunning) {

        gui.SetScore(leftScore, rightScore);
        gui.RenderScreen(leftPaddle, rightPaddle, ball);

        while (isStartScreen) {

            while (SDL_WaitEvent(&event)) {
                HandleStartEvents(event, leftPaddle, rightPaddle, ball, gui);
                break;
            }
        }

        Uint32 frameStart {};

        while (isInPlay) {

            // Get the time of the start of the loop
            frameStart = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                HandleKeyboardEvents(event, leftPaddle, rightPaddle, ball, gui);
                break;
            }

            // Moving, collision, and leaving the court
            HandleBallEvents(leftPaddle, rightPaddle, ball, gui);

            gui.RenderScreen(leftPaddle, rightPaddle, ball);

            // Limit frame rate to desired FPS
            int frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < FRAME_DELAY) {
                SDL_Delay(FRAME_DELAY - frameTime);
            }
        }
    }
}