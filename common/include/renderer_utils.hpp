#ifndef RENDER_RENDERER_UTILS_HPP
#define RENDER_RENDERER_UTILS_HPP

#include "config.hpp"
#include "vector.hpp"

#include <cmath>
#include <vector>

namespace render {

  // Color utilities
  [[nodiscard]] vector gamma_correct(const vector& color, double gamma);
  [[nodiscard]] vector clamp_color(const vector& color);
  [[nodiscard]] int color_to_int(double component);

  // PPM output
  void write_ppm(const std::string& filename, const std::vector<std::vector<vector>>& image, int width, int height);

}  // namespace render

#endif

