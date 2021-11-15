#include "Vector2d.hpp"
#include "Graphics.h"
#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"
Paddle::Paddle(Vector2d start, Vector2d size, Vector2d _n) : x(start), v(Vector2d(paddleSpeed, 0)), paddleSize(size), drawnX(start),
  n(_n)
{

}

void Paddle::move(int dir, double dt)
{
  if (dir == 0) {
    Serial.println("going down");
    v.setX(-paddleSpeed);
  }
  else if (dir == 1) {
    Serial.println("going up");
    v.setX(paddleSpeed);
  }
  else {
    v.setX(0);
  }
  // calculate new position
  x += v * dt;
}

void Paddle::move(Vector2d vel, double dt)
{
  if (vel.getX() > 0)
    v.setX(paddleSpeed);
  if (vel.getX() < 0)
    v.setX(-paddleSpeed);

  // calculate new position
  x += v * dt;
}

void Paddle::draw()
{
  // erase old location first
  fillRect(drawnX, paddleSize, BLACK);

  // draw paddle as a rectangle
  fillRect(x, paddleSize, RED);

  // remember where we drew it
  drawnX = x;
}

void Paddle::handleCollision(const Wall* wall)
{
  Vector2d n = wall->getNormal();

  // move off the wall a little
  x += 2 * n;
}

bool Paddle::isCollidingWith(const Wall* wall)
{
  // is ball to left of wall?
  Vector2d n = wall->getNormal();
  Vector2d p1 = wall->getStart();
  Vector2d p2 = Vector2d(p1.getX() - paddleSize.getX(), p1.getY());

  return ( ((x - p1).dot(n) < 0) || ((x - p2).dot(n) < 0) );
}

const Vector2d Paddle::getNormal() const
{
  return n;
}

const Vector2d Paddle::getUl() const
{
  return x;
}

const Vector2d Paddle::getLr() const
{
  return x + paddleSize;
}
