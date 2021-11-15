#pragma once

#include "Graphics.h"

#include "Vector2d.hpp"

class Wall
{
  public:
    Wall(const Vector2d& p1, const Vector2d& p2, uint16_t color);
    void draw() const;

    const Vector2d getNormal() const;
    const Vector2d getStart() const;
    const Vector2d getEnd() const;

  private:
    Vector2d start;    // endpoints
    Vector2d end;
    uint16_t wallColor;

    Vector2d n;     // normal vector
};
