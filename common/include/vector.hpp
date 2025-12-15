#ifndef RENDER_VECTOR_HPP
#define RENDER_VECTOR_HPP

#include <cmath>

namespace render {

  class vector {
  public:
    vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} {}
    vector() : x{0.0}, y{0.0}, z{0.0} {}

    [[nodiscard]] double magnitude() const;
    [[nodiscard]] double magnitude_squared() const;
    [[nodiscard]] vector normalize() const;
    [[nodiscard]] double dot(const vector& other) const;
    [[nodiscard]] vector cross(const vector& other) const;
    
    [[nodiscard]] double get_x() const { return x; }
    [[nodiscard]] double get_y() const { return y; }
    [[nodiscard]] double get_z() const { return z; }

    vector operator+(const vector& other) const;
    vector operator-(const vector& other) const;
    vector operator*(double scalar) const;
    vector operator/(double scalar) const;
    vector operator-() const;
    
    vector& operator+=(const vector& other);
    vector& operator-=(const vector& other);
    vector& operator*=(double scalar);
    vector& operator/=(double scalar);

  private:
    double x, y, z;
  };

  vector operator*(double scalar, const vector& v);

}

#endif
