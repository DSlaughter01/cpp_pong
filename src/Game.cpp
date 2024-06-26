#include "Game.hpp"

bool GameVariables::isInPlay = false;
bool GameVariables::isGameOver = false;

Game::Game() :
    isRunning(true), leftScore(0), rightScore(0), resize(false)
{
    // Initialise SDL, handling errors as needed
    SDL_Init(SDL_INIT_EVERYTHING);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL has not been initialised properly: " << SDL_GetError() << std::endl;
    }
}

Game::~Game() {}

void Game::HandleKeysInGame(const Uint8 * currentKeyboardState, SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle) {

    // Right paddle movement
    if (currentKeyboardState[SDL_SCANCODE_UP] && 
        rightPaddle.rect.y > courtPadding)
        rightPaddle.SetVelocity('u');

    else if (currentKeyboardState[SDL_SCANCODE_DOWN] && 
             rightPaddle.rect.y + rightPaddle.rect.h < windowHeight - courtPadding) 
        rightPaddle.SetVelocity('d');

    else
        rightPaddle.SetVelocity('n');

    // Left paddle movement
    if (currentKeyboardState[SDL_SCANCODE_W] && 
        leftPaddle.rect.y > courtPadding)
        leftPaddle.SetVelocity('u'); 

    else if (currentKeyboardState[SDL_SCANCODE_S] && 
             leftPaddle.rect.y + leftPaddle.rect.h < windowHeight - courtPadding)  
        leftPaddle.SetVelocity('d');

    else
        leftPaddle.SetVelocity('n');
}


void Game::HandleCollisions(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle) {

    // Check for collision with paddles
    if ((ball.rect.x + ball.rect.w >= rightPaddle.rect.x && ball.dx > 0 && SDL_HasIntersection(&ball.rect, &rightPaddle.rect)) 
    ||
    (ball.rect.x <= leftPaddle.rect.x + leftPaddle.rect.w && ball.dx < 0 && SDL_HasIntersection(&ball.rect, &leftPaddle.rect)))
        ball.BounceOffPaddle();

    // Check for collision with court edges
    if ((ball.rect.y <= GameVariables::courtTopY && ball.dy < 0) || 
        (ball.rect.y + ball.rect.h >= GameVariables::courtBottomY && ball.dy > 0))
        ball.ReverseDY();
}


void Game::MoveGameObjects(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle) {

    leftPaddle.Move();
    rightPaddle.Move();
    ball.Move();
}


void Game::HandleWindowResize(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, GUI &gui) {

    // Get the new window dimensions and set them in GameVariables
    // Calculate the difference in the old and new sizes
    std::pair<int, int> dimensionChanges = gui.ChangeWindowSize();

    // Update GameVariable file to reflect changes
    courtRightX += dimensionChanges.first;
    courtBottomY += dimensionChanges.second;

    // Reposition game objects accordingly
    ball.ReactToWindowResize(dimensionChanges.first, dimensionChanges.second);
    leftPaddle.ReactToWindowResize(dimensionChanges.first, dimensionChanges.second);
    rightPaddle.ReactToWindowResize(dimensionChanges.first, dimensionChanges.second);
}


void Game::HandleScore(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, char whoScored, GUI &gui) {

    // Check that the correct arguments are passed to the function
    if (whoScored != 'l' && whoScored != 'r') {
        std::cerr << "Must pass either 'l' or 'r' as whoScored argument" << std::endl;
        return;
    }
    
    // Reset game objects and set the ball to stationary
    ball.Reset();
    leftPaddle.Reset();
    rightPaddle.Reset();
    isInPlay = false;

    // Update score
    if (whoScored == 'l') 
        leftScore++;
    else if (whoScored == 'r')
        rightScore++;
    gui.UpdateScoreTex(leftScore, rightScore);

    // Handle game over
    if (leftScore == 10)
        HandleGameOver(ball, leftPaddle, rightPaddle, 'l', gui);
    else if (rightScore == 10)
        HandleGameOver(ball, leftPaddle, rightPaddle, 'r', gui);
}


void Game::HandleGameOver(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, char winner, GUI &gui) {

    isGameOver = true;
    gui.GenerateGameOverScreen(winner);
}


void Game::ResetToNewGame(Ball &ball, Paddle &leftPaddle, Paddle &rightPaddle, GUI &gui) {

    isGameOver = false;

    // Reset game objects and set the ball to stationary
    ball.Reset();
    leftPaddle.Reset();
    rightPaddle.Reset();
    isInPlay = false;

    // Reset scores
    leftScore = 0;
    rightScore = 0;
    gui.UpdateScoreTex(leftScore, rightScore);
}


void Game::GameLoop(GUI gui) {

    Uint32 frameStart, frameEnd;
    SDL_Event event;

    // Gets all of the keys pressed simultaneously
    const Uint8 *currentKeyboardState = SDL_GetKeyboardState(NULL);

    // Create game objects on the stack in the GameLoop scope
    Ball ball;
    Paddle leftPaddle = Paddle('l');
    Paddle rightPaddle = Paddle('r');

    while (isRunning) {

        frameStart = SDL_GetTicks64();

        if (SDL_PollEvent(&event)) {

            // User quits the game
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                isRunning = false;
                break;
            }

            // Spacebar starts ball movement
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE && !isInPlay) {
                    isInPlay = true;
                    ball.StartMovement();
                }
            }

            if (event.window.event && SDL_WINDOWEVENT_SIZE_CHANGED){
                resize = true;
            }
        }

        if (resize) {
            HandleWindowResize(ball, leftPaddle, rightPaddle, gui);
            resize = false;
        }

        // Handle and display movement
        HandleKeysInGame(currentKeyboardState, event, leftPaddle, rightPaddle);
        MoveGameObjects(ball, leftPaddle, rightPaddle);
        HandleCollisions(ball, leftPaddle, rightPaddle);

        // One player scores if the ball goes off the court
        if (ball.rect.x + ball.rect.w < 0)
            HandleScore(ball, leftPaddle, rightPaddle, 'r', gui);
        else if (ball.rect.x > windowWidth)
            HandleScore(ball, leftPaddle, rightPaddle, 'l', gui);

        // Render the screen
        gui.RenderScreen(ball, leftPaddle, rightPaddle);

        while (isGameOver) {

            if (SDL_WaitEvent(&event)) {

                // User quits the game
                if (event.type == SDL_QUIT) {
                    SDL_Quit();
                    isRunning = false;
                    break;
                }
                // Wait for event press enter to start a new game
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        ResetToNewGame(ball, leftPaddle, rightPaddle, gui);
                        break;
                    }
                }
            }
        }

        // Control frame rate
        frameEnd = SDL_GetTicks64();
        if (frameEnd - frameStart < desiredFrameTicks)
            SDL_Delay(desiredFrameTicks - (frameEnd - frameStart));
    }
}