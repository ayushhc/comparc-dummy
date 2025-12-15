#ifndef RENDER_CONFIG_HPP
#define RENDER_CONFIG_HPP

#include "vector.hpp"

#include <string>
#include <vector>

namespace render {

  struct render_config {
    // Image parameters
    int image_width = 800;
    int aspect_ratio_width = 16;
    int aspect_ratio_height = 9;
    double gamma = 2.2;

    // Camera parameters
    vector camera_position{0.0, 0.0, 0.0};
    vector camera_target{0.0, 0.0, -1.0};
    vector camera_north{0.0, 1.0, 0.0};
    double field_of_view = 90.0;

    // Rendering parameters
    int samples_per_pixel = 100;
    int max_depth = 50;
    unsigned int material_rng_seed = 0;
    unsigned int ray_rng_seed = 0;

    // Background parameters
    vector background_dark_color{0.5, 0.7, 1.0};
    vector background_light_color{1.0, 1.0, 1.0};
  };

  class config_parser {
  public:
    [[nodiscard]] static render_config parse(const std::string& filename);

  private:
    static std::vector<std::string> split_line(const std::string& line);
    static bool is_whitespace(const std::string& line);
    static void parse_parameter(render_config& config, const std::string& key, const std::vector<std::string>& values, const std::string& line);
  };

}  // namespace render

#endif

