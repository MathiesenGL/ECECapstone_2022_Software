#include "Vector2d.hpp"
#include "Graphics.h"
#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"

// Constructor
Ball::Ball(Vector2d _x) : x(_x), v(Vector2d(100, 100)), drawnX(_x)
{
  // intentionally blank
}

void Ball::move(double dt)
{
  x += v * dt;
}

void Ball::draw()
{
  // erase old location first
  fillCircle(drawnX, 2, BLACK);

  // draw ball as a circle
  fillCircle(x, 2, RED);

  // remember where we drew it
  drawnX = x;
}

void Ball::handleCollision(const Wall* wall)
{
  Vector2d n = wall->getNormal();

  // move off the wall a little
  x += 2 * n;

  // change direction
  v -= 2 * v.dot(n) * n;
}

bool Ball::isCollidingWith(const Wall* wall)
{
  // is ball to left of wall?
  Vector2d n = wall->getNormal();
  Vector2d p1 = wall->getStart();

  return ( (x - p1).dot(n) < 0 );
}
void Ball::handleCollision(const Paddle* paddle)
{
  Vector2d n = paddle->getNormal();

  // move off the wall a little
  x += 2 * n;

  // change direction
  v -= 2 * v.dot(n) * n;
}

bool Ball::isCollidingWith(const Paddle* paddle)
{
  // is ball to left of wall?
  Vector2d pUl = paddle->getUl();
  Vector2d pLr = paddle->getLr();

  return (x.getX() < pLr.getX() && x.getY() < pLr.getY()) && (pUl.getX() < x.getX() && pUl.getY() < x.getY());
}

Vector2d Ball::getV()
{
  return v;
}
