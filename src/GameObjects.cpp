#include "GameObjects.hpp"

Paddle::Paddle(char side) :
    w(20), h(75), dy(0), side(side)
{
    // Set x depending on whether the paddle is left or right
    x = (side == 'l') ? courtPadding : windowWidth - courtPadding - w;
    y = (windowHeight - h) / 2;

    rect = {x, y, w, h};
}


void Paddle::Reset() {

    x = (side == 'l') ? courtPadding : windowWidth - courtPadding - w;
    rect = {x, y, w, h};
    dy = 0;
}


void Paddle::SetVelocity(char direction) {
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


void Paddle::ReactToWindowResize(int windowWidthChange, int windowHeightChange) {

    if (side == 'r') 
        rect.x += windowWidthChange;

    // Make sure the paddle doesn't go beyond the top or bottom court lines
    if (rect.y + windowHeightChange / 2 < courtTopY) 
        rect.y = courtTopY;
    else if (rect.y + windowHeightChange + rect.h > courtBottomY)
        rect.y = courtBottomY - rect.h;
    else
        rect.y += windowHeightChange / 2;
}


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
    x = (windowWidth - w) / 2;
    y = (windowHeight - h) / 2;
    rect = {x, y, w, h};
    dx = 0;
    dy = 0;
}


void Ball::Move() {
    rect.x += dx;
    rect.y += dy;
}


void Ball::ReverseDY() {
    dy *= -1;
}


void Ball::BounceOffPaddle() {
    if (dx < maxBallVel)
        dx *= -1.1;
}


void Ball::ReactToWindowResize(int windowWidthChange, int windowHeightChange) {

    rect.x += windowWidthChange / 2;
    rect.y += windowHeightChange / 2;
}
