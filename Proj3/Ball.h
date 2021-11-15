#pragma once

#include "Vector2d.hpp"
#include "Wall.h"
#include "Paddle.h"

class Ball
{
  public:
    Ball(Vector2d _x);
    void move(double dt);
    void draw();
    bool isCollidingWith(const Wall* wall);
    void handleCollision(const Wall* wall);
    bool isCollidingWith(const Paddle* paddle);
    void handleCollision(const Paddle* paddle);
    Vector2d getV();

  private:
    Vector2d x;     // position
    Vector2d v;     // velocity

    Vector2d drawnX;    // where we're drawn on the screen
};
