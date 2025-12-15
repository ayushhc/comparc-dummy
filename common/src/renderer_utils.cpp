#include "renderer_utils.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

namespace render {

  vector gamma_correct(const vector& color, double gamma) {
    const double inv_gamma = 1.0 / gamma;
    return vector{
      std::pow(color.get_x(), inv_gamma),
      std::pow(color.get_y(), inv_gamma),
      std::pow(color.get_z(), inv_gamma)
    };
  }

  vector clamp_color(const vector& color) {
    return vector{
      std::max(0.0, std::min(1.0, color.get_x())),
      std::max(0.0, std::min(1.0, color.get_y())),
      std::max(0.0, std::min(1.0, color.get_z()))
    };
  }

  int color_to_int(double component) {
    return static_cast<int>(255.999 * component);
  }

  void write_ppm(const std::string& filename, const std::vector<std::vector<vector>>& image, int width, int height) {
    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Error: Could not open output file: " + filename);
    }

    file << "P3\n";
    file << width << " " << height << "\n";
    file << "255\n";

    for (int j = height - 1; j >= 0; --j) {
      for (int i = 0; i < width; ++i) {
        const vector& color = image[i][j];
        file << color_to_int(color.get_x()) << " "
             << color_to_int(color.get_y()) << " "
             << color_to_int(color.get_z()) << "\n";
      }
    }
  }

}

