#include "GameObjects.hpp"

Paddle::Paddle(char side) :
    w(20), h(75), dy(0)
{
    x = (side == 'l') ? courtPadding : windowWidth - courtPadding - w;
    y = (windowHeight - h) / 2;

    rect = {x, y, w, h};
}

void Paddle::Reset() {
    rect = {x, y, w, h};
    dy = 0;
}

// direction = 'u' (up), 'd' (down), or 'n' (none)
void Paddle::SetDirection(char direction) {
    switch (direction) {
        case 'u' : 
            dy = -paddleVel;
            break;
        case 'd' : 
            dy = paddleVel;
            break;
        case 'n' :
            dy = 0;
            break;
    }
}

void Paddle::Move() {rect.y += dy;}

Ball::Ball() :
    w(20), h(20), dx(0), dy(0)
{
    x = (windowWidth - w) / 2;
    y = (windowHeight - h) / 2;
    rect = {x, y, w, h};
}

void Ball::StartMovement() {
    dx = initialBallDX;

    // TODO: Randomly generate dy between -2 and 2
    dy = 2;
}

void Ball::Reset() {
    rect = {x, y, w, h};
    dx = 0;
    dy = 0;
}

void Ball::Move() {
    rect.x += dx;
    rect.y += dy;
}

void Ball::ReverseDY() {dy *= -1;}

// This function is called on the relevant paddle under conditions seen in the Game class
void Ball::CheckForPaddleCollision(Paddle &paddle) {
    if (SDL_HasIntersection(&rect, &paddle.rect))
        dx *= -1.1;
}