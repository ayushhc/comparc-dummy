#include "camera.hpp"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace render {

  camera::camera(const render_config& config) {
    image_width_ = config.image_width;
    image_height_ = static_cast<int>(static_cast<double>(image_width_) * 
                      static_cast<double>(config.aspect_ratio_height) / 
                      static_cast<double>(config.aspect_ratio_width));

    origin_ = config.camera_position;
    const vector target = config.camera_target;
    const vector north = config.camera_north;

    vector forward = (target - origin_).normalize();
    vector right = forward.cross(north).normalize();
    vector up = right.cross(forward).normalize();

    const double aspect_ratio = static_cast<double>(image_width_) / static_cast<double>(image_height_);
    const double theta = config.field_of_view * M_PI / 180.0;
    const double h = std::tan(theta / 2.0);
    const double viewport_height = 2.0 * h;
    const double viewport_width = aspect_ratio * viewport_height;

    horizontal_ = right * viewport_width;
    vertical_ = up * viewport_height;

    lower_left_corner_ = origin_ + forward - horizontal_ / 2.0 - vertical_ / 2.0;
  }

  ray camera::get_ray(double u, double v) const {
    const vector direction = lower_left_corner_ + horizontal_ * u + vertical_ * v - origin_;
    return ray(origin_, direction.normalize());
  }

}

