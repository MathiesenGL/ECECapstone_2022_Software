#ifndef VMI_GAME_VECTOR_H
#define VMI_GAME_VECTOR_H

#include <cmath>

class Vector2d
{
  public:
    // Constructors
    Vector2d() : x(0), y(0) { }
    Vector2d(double _x, double _y) : x(_x), y(_y) { }

    // Dot product
    double dot(const Vector2d& v) const
    {
      return x * v.x + y * v.y;
    }

    // Vector magnitude
    double magnitude() const
    {
      return std::sqrt(x * x + y * y);
    }

    // Nomralized vector
    Vector2d normalize() const
    {
      double m = magnitude();
      return (m == 0.0) ? Vector2d() : Vector2d(x / m, y / m);
    }

    // getters and setters
    double getX() const
    {
      return x;
    }

    double getY() const
    {
      return y;
    }

    void setX(double _x)
    {
      x = _x;
    }

    void setY(double _y)
    {
      y = _y;
    }

    // overloaded operators -- assignment operators
    Vector2d& operator+=(const Vector2d& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    Vector2d& operator-=(const Vector2d& rhs)
    {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
    }

    Vector2d& operator*=(double c)
    {
      x *= c;
      y *= c;
      return *this;
    }

    // overloaded operators -- arithmetic
    friend Vector2d operator +(Vector2d lhs, const Vector2d& rhs)
    { // addition
      lhs += rhs;
      return lhs;
    }

    friend Vector2d operator -(Vector2d lhs, const Vector2d& rhs)
    { // subtraction
      lhs -= rhs;
      return lhs;
    }

    friend Vector2d operator *(Vector2d lhs, double rhs)
    { // vector*double
      lhs *= rhs;
      return lhs;
    }

    friend Vector2d operator *(double lhs, Vector2d rhs)
    { // double*vector
      rhs *= lhs;
      return rhs;
    }

    Vector2d operator-()
    {
      return Vector2d(-x, -y);  // unary minus
    }

    // overloaded operators -- comparisons
    friend bool operator ==(const Vector2d& v1, const Vector2d& v2)
    {
      return (v1.x == v2.x) && (v1.y == v2.y);
    }

    friend bool operator !=(const Vector2d& v1, const Vector2d& v2)
    {
      return !(v1 == v2);
    }


  private:
    double x = 0.0;
    double y = 0.0;
};


#endif
