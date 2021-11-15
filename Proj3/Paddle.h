#pragma once

#include "Graphics.h"

#include "Vector2d.hpp"
#include "Wall.h"

class Paddle
{
  public:
    Paddle(Vector2d start, Vector2d size, Vector2d _n);
    void move(int dir, double dt);
    void move(Vector2d vel, double dt);
    void draw();
    bool isCollidingWith(const Wall* wall);
    void handleCollision(const Wall* wall);
    const Vector2d getNormal() const;
    const Vector2d getUl() const;
    const Vector2d getLr() const;

  private:
    Vector2d x;     // position
    Vector2d v;     // current velocity
    Vector2d paddleSize;  // size of paddle
    Vector2d n;     // normal vector from paddle

    Vector2d drawnX;    // where we're drawn on the screen
    const double paddleSpeed = 400.0;   // how fast the paddle can move
};
