#include "Graphics.h"
#include "Vector2d.hpp"
#include "Wall.h"

// Constructor -- save the endpoints and determine the normal vector
Wall::Wall(const Vector2d& p1, const Vector2d& p2, uint16_t color)
  : start(p1), end(p2), wallColor(color)
{
  Vector2d diff = (end - start).normalize();
  n = Vector2d(-diff.getY(), diff.getX());
}

// Getter functions
const Vector2d Wall::getStart() const
{
  return start;
}

const Vector2d Wall::getEnd() const
{
  return end;
}

const Vector2d Wall::getNormal() const
{
  return n;
}

// Draw the wall
void Wall::draw() const
{
  drawLine(start, end, wallColor);
}
