#include "GUI.hpp"

SDL_Window* GUI::window;
SDL_Renderer* GUI::renderer;

GUI::GUI()
    : textColour({230, 230, 230}), startText("Press space to start"), lScoreTxr(nullptr),
    rScoreTxr(nullptr), startTextTxr(nullptr) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL initialisation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }   

    // Create window
    window = SDL_CreateWindow("Pong", 
                              SDL_WINDOWPOS_UNDEFINED, 
                              SDL_WINDOWPOS_UNDEFINED,
                              winWidth,
                              winHeight,
                              SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "Window initialisation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }                    
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        SDL_DestroyWindow(window);
        std::cerr << "Renderer initialisation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }    

    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);    

    // Initialise GUI variables
    courtRect = {currentPadding, currentPadding, (winWidth - 2 * currentPadding), (winHeight - 2 * currentPadding)};

    // Text
    if (TTF_Init() < 0) {
        std::cerr << "TTF initialisation error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        return;
    } 
    
    // Should work out how to resize based on window width and height
    // Maybe in a while loop while the text width is bigger than 50% (say) of the screen
    startFont = TTF_OpenFont("src/Roboto-Regular.ttf", 32);
    if (!startFont) {
        std::cerr << "Problem initialising start font: " << TTF_GetError() << std::endl;
        return;
    }

    scoreFont = TTF_OpenFont("src/Roboto-Regular.ttf", 50);
    if (!scoreFont) {
        std::cerr << "Problem initialising score font: " << TTF_GetError() << std::endl;
        return;
    }
}


GUI::~GUI() {

    SDL_DestroyTexture(lScoreTxr);
    SDL_DestroyTexture(rScoreTxr);
    SDL_DestroyTexture(startTextTxr);

    lScoreTxr = nullptr;
    rScoreTxr = nullptr;
    startTextTxr = nullptr; 

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    window = nullptr;
    renderer = nullptr;

    SDL_Quit();
}


void GUI::SetScore(int leftScore, int rightScore) {

    SDL_RenderClear(renderer);

    std::stringstream ls;
    std::stringstream rs;
    ls << leftScore;
    rs << rightScore;

    // Starting text texture
    SDL_Surface* startTextSfc = TTF_RenderText_Solid(startFont, startText, textColour);
    if (!startTextSfc) {
        std::cerr << "Problem with surface creation" << SDL_GetError() << std::endl;
        return;
    }
    startTextTxr = SDL_CreateTextureFromSurface(renderer, startTextSfc);
    if (!startTextTxr) {
        std::cerr << "Problem with texture creation" << SDL_GetError() << std::endl;
        return;
    }

    // Starting text rectangle
    startTextRect = {(winWidth - startTextSfc->w) / 2, 
                      winHeight / 4 - startTextSfc->h / 2, 
                      startTextSfc->w, startTextSfc->h};

    SDL_FreeSurface(startTextSfc);

    // Left and right score surfaces
    SDL_Surface* lScoreSfc = TTF_RenderText_Solid(scoreFont, ls.str().c_str(), textColour);
    SDL_Surface* rScoreSfc = TTF_RenderText_Solid(scoreFont, rs.str().c_str(), textColour);
    if (!lScoreSfc || !rScoreSfc) {
        std::cerr << "Problem with score creation" << TTF_GetError() << std::endl;
        SDL_Quit();
    }
    
    // L & R score textures
    lScoreTxr = SDL_CreateTextureFromSurface(renderer, lScoreSfc);
    rScoreTxr = SDL_CreateTextureFromSurface(renderer, rScoreSfc);
    if (!lScoreTxr || !rScoreTxr) {
        std::cerr << "Problem with score texture" << TTF_GetError() << std::endl;
        SDL_Quit();
    }

    // L & R rectangles
    lScoreRect = {winWidth / 4 - lScoreSfc->w, 
                 (winHeight - lScoreSfc->h) / 2,
                  lScoreSfc->h, lScoreSfc->w};

    rScoreRect = {3 * winWidth / 4 - rScoreSfc->w,
                 (winHeight - rScoreSfc->h) / 2,
                  rScoreSfc->h, rScoreSfc->w};

    // Free resources
    SDL_FreeSurface(lScoreSfc);
    SDL_FreeSurface(rScoreSfc);

    // Render all 3 bits of text
    SDL_RenderCopy(renderer, startTextTxr, NULL, &startTextRect);
    SDL_RenderCopy(renderer, lScoreTxr, NULL, &lScoreRect);
    SDL_RenderCopy(renderer, rScoreTxr, NULL, &rScoreRect);

    SDL_RenderPresent(renderer);
}


void GUI::RenderCourt(Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball) {

    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);  

    // Scores
    SDL_RenderCopy(renderer, lScoreTxr, NULL, &lScoreRect);
    SDL_RenderCopy(renderer, rScoreTxr, NULL, &rScoreRect);

    // Draw outside lines
    SDL_RenderDrawRect(renderer, &courtRect);

    // Draw paddles
    SDL_RenderFillRect(renderer, &(leftPaddle.paddleRect));
    SDL_RenderFillRect(renderer, &(rightPaddle.paddleRect));

    // Draw ball
    SDL_RenderFillRect(renderer, &(ball.ballRect));

    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
}


void GUI::RenderScreen(Paddle &leftPaddle, Paddle &rightPaddle, Ball &ball) {

    SDL_RenderClear(renderer);
    RenderCourt(leftPaddle, rightPaddle, ball);
    SDL_RenderPresent(renderer);
}

void GUI::HandleWindowResize(SDL_Event &event, Paddle &leftPaddle, Paddle &rightPaddle) {

    // Adjust the window width and padding
    winWidth = event.window.data1;
    winHeight = event.window.data2;
    currentPadding = initialPadding + winHeight / 20;

    // Set court rectangle coordinates
    courtRect = {currentPadding, currentPadding, winWidth - 2 * currentPadding, winHeight - 2 * currentPadding};

    // Change paddle x and y
    paddleLX = currentPadding + paddlePadding;
    paddleRX = winWidth - currentPadding - paddlePadding - paddleW;
    paddleY = (winHeight - paddleH) / 2;

    // Set limits for paddle movement
    leftPaddle.yBot = winHeight - currentPadding;
    leftPaddle.yTop = currentPadding;

    // Update paddle rectangle
    leftPaddle.paddleRect = {paddleLX, paddleY, paddleRX, paddleY};
}