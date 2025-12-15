#include "vector.hpp"

#include <cmath>

namespace render {

  double vector::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  double vector::magnitude_squared() const {
    return x * x + y * y + z * z;
  }

  vector vector::normalize() const {
    const double mag = magnitude();
    if (mag == 0.0) {
      return vector{0.0, 0.0, 0.0};
    }
    return vector{x / mag, y / mag, z / mag};
  }

  double vector::dot(const vector& other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  vector vector::cross(const vector& other) const {
    return vector{
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    };
  }

  vector vector::operator+(const vector& other) const {
    return vector{x + other.x, y + other.y, z + other.z};
  }

  vector vector::operator-(const vector& other) const {
    return vector{x - other.x, y - other.y, z - other.z};
  }

  vector vector::operator*(double scalar) const {
    return vector{x * scalar, y * scalar, z * scalar};
  }

  vector vector::operator/(double scalar) const {
    return vector{x / scalar, y / scalar, z / scalar};
  }

  vector vector::operator-() const {
    return vector{-x, -y, -z};
  }

  vector& vector::operator+=(const vector& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  vector& vector::operator-=(const vector& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  vector& vector::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  vector& vector::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  vector operator*(double scalar, const vector& v) {
    return v * scalar;
  }

}
