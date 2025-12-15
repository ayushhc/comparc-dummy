#ifndef RENDER_CAMERA_HPP
#define RENDER_CAMERA_HPP

#include "config.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <cmath>

namespace render {

  class camera {
  public:
    camera(const render_config& config);

    [[nodiscard]] ray get_ray(double u, double v) const;
    [[nodiscard]] int get_image_width() const { return image_width_; }
    [[nodiscard]] int get_image_height() const { return image_height_; }

  private:
    vector origin_;
    vector lower_left_corner_;
    vector horizontal_;
    vector vertical_;
    int image_width_;
    int image_height_;
  };

}

#endif

