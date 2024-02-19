#include "GameObjects.hpp"
#include <iostream>

Paddle::Paddle(int x, int y, int w, int h, int yTop, int yBot)
    : initX(x), initY(y), dy(4), yTop(yTop), yBot(yBot) {

    paddleRect = {x, y, w, h};
}

void Paddle::MoveDown() {
    if (paddleRect.y + paddleRect.h <= yBot)
        paddleRect.y += dy;
}

void Paddle::MoveUp() {
    if (paddleRect.y >= yTop)
        paddleRect.y -= dy;
}

void Paddle::Reset() {
    paddleRect.x = initX;
    paddleRect.y = initY;
}

Ball::Ball(int yTop, int yBot, int winWidth) 
    : ballDim(20), dx(1), dy(1), yTop(yTop), yBot(yBot), winWidth(winWidth) {

    initX = (winWidth - ballDim) / 2;
    initY = (winHeight - ballDim) / 2;
    ballRect = {initX, initY, ballDim, ballDim};
}

void Ball::Reset() {
    // Position
    ballRect.x = initX;
    ballRect.y = initY;

    // Velocity
    dy = 1;
}

void Ball::Move() {
    ballRect.x += dx;
    ballRect.y += dy;
}

void Ball::HandleCollision(Paddle &leftPaddle, Paddle &rightPaddle) {

    // Handle hitting the top or bottom
    if (ballRect.y <= yTop || ballRect.y + ballRect.h >= yBot)
        dy *= -1;
    
    // Paddle intersection - set the ball outside of the paddle to prevent multiple collisions

    else if (SDL_HasIntersection(&ballRect, &(rightPaddle.paddleRect))) {

        ballRect.x = rightPaddle.paddleRect.x - ballRect.w - 1;
        dx *= -1.2;
    }

    else if (SDL_HasIntersection(&ballRect, &(leftPaddle.paddleRect))) {
        ballRect.x = leftPaddle.paddleRect.x + leftPaddle.paddleRect.w + 1;
        dx *= -1.2;
    }
}

// Has the ball left the court? Returns 1 for leave right, -1 for leave left, or 0 for not out
int Ball::IsOut() {
    // Point for left player if ball goes off right side
    if (ballRect.x >= winWidth)
        return 1;
        
    // Opposite for right player
    else if (ballRect.x + ballRect.w <= 0)
        return -1;

    else
        return 0;
}