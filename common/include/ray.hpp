#ifndef RENDER_RAY_HPP
#define RENDER_RAY_HPP

#include "vector.hpp"

namespace render {

  class ray {
  public:
    ray(const vector& origin, const vector& direction)
      : origin_{origin}, direction_{direction} {}

    [[nodiscard]] const vector& get_origin() const { return origin_; }
    [[nodiscard]] const vector& get_direction() const { return direction_; }
    [[nodiscard]] vector point_at(double t) const {
      return origin_ + direction_ * t;
    }

  private:
    vector origin_;
    vector direction_;
  };

}  // namespace render

#endif

