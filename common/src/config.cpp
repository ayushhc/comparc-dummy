#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace render {

  std::vector<std::string> config_parser::split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
      tokens.push_back(token);
    }
    
    return tokens;
  }

  bool config_parser::is_whitespace(const std::string& line) {
    for (char c : line) {
      if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
        return false;
      }
    }
    return true;
  }

  void config_parser::parse_parameter(render_config& config, const std::string& key, const std::vector<std::string>& values, const std::string& line) {
    if (key == "image_width:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid image_width parameters\nLine: \"" + line + "\"");
      }
      config.image_width = std::stoi(values[0]);
      if (config.image_width <= 0) {
        throw std::runtime_error("Error: Invalid image_width parameters\nLine: \"" + line + "\"");
      }
    }
    else if (key == "aspect_ratio:") {
      if (values.size() != 2) {
        throw std::runtime_error("Error: Invalid aspect_ratio parameters\nLine: \"" + line + "\"");
      }
      config.aspect_ratio_width = std::stoi(values[0]);
      config.aspect_ratio_height = std::stoi(values[1]);
      if (config.aspect_ratio_width <= 0 || config.aspect_ratio_height <= 0) {
        throw std::runtime_error("Error: Invalid aspect_ratio parameters\nLine: \"" + line + "\"");
      }
    }
    else if (key == "gamma:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid gamma parameters\nLine: \"" + line + "\"");
      }
      config.gamma = std::stod(values[0]);
    }
    else if (key == "camera_position:") {
      if (values.size() != 3) {
        throw std::runtime_error("Error: Invalid camera_position parameters\nLine: \"" + line + "\"");
      }
      config.camera_position = vector{
        std::stod(values[0]),
        std::stod(values[1]),
        std::stod(values[2])
      };
    }
    else if (key == "camera_target:") {
      if (values.size() != 3) {
        throw std::runtime_error("Error: Invalid camera_target parameters\nLine: \"" + line + "\"");
      }
      config.camera_target = vector{
        std::stod(values[0]),
        std::stod(values[1]),
        std::stod(values[2])
      };
    }
    else if (key == "camera_north:") {
      if (values.size() != 3) {
        throw std::runtime_error("Error: Invalid camera_north parameters\nLine: \"" + line + "\"");
      }
      config.camera_north = vector{
        std::stod(values[0]),
        std::stod(values[1]),
        std::stod(values[2])
      };
    }
    else if (key == "field_of_view:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid field_of_view parameters\nLine: \"" + line + "\"");
      }
      config.field_of_view = std::stod(values[0]);
    }
    else if (key == "samples_per_pixel:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid samples_per_pixel parameters\nLine: \"" + line + "\"");
      }
      config.samples_per_pixel = std::stoi(values[0]);
      if (config.samples_per_pixel <= 0) {
        throw std::runtime_error("Error: Invalid samples_per_pixel parameters\nLine: \"" + line + "\"");
      }
    }
    else if (key == "max_depth:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid max_depth parameters\nLine: \"" + line + "\"");
      }
      config.max_depth = std::stoi(values[0]);
      if (config.max_depth <= 0) {
        throw std::runtime_error("Error: Invalid max_depth parameters\nLine: \"" + line + "\"");
      }
    }
    else if (key == "material_rng_seed:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid material_rng_seed parameters\nLine: \"" + line + "\"");
      }
      config.material_rng_seed = static_cast<unsigned int>(std::stoul(values[0]));
    }
    else if (key == "ray_rng_seed:") {
      if (values.size() != 1) {
        throw std::runtime_error("Error: Invalid ray_rng_seed parameters\nLine: \"" + line + "\"");
      }
      config.ray_rng_seed = static_cast<unsigned int>(std::stoul(values[0]));
    }
    else if (key == "background_dark_color:") {
      if (values.size() != 3) {
        throw std::runtime_error("Error: Invalid background_dark_color parameters\nLine: \"" + line + "\"");
      }
      config.background_dark_color = vector{
        std::stod(values[0]),
        std::stod(values[1]),
        std::stod(values[2])
      };
    }
    else if (key == "background_light_color:") {
      if (values.size() != 3) {
        throw std::runtime_error("Error: Invalid background_light_color parameters\nLine: \"" + line + "\"");
      }
      config.background_light_color = vector{
        std::stod(values[0]),
        std::stod(values[1]),
        std::stod(values[2])
      };
    }
    else {
      throw std::runtime_error("Error: Unknown configuration parameter: " + key);
    }
  }

  render_config config_parser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Error: Could not open config file: " + filename);
    }

    render_config config;
    std::string line;

    while (std::getline(file, line)) {
      // Remove trailing whitespace
      while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
        line.pop_back();
      }

      // Skip empty lines
      if (is_whitespace(line)) {
        continue;
      }

      std::vector<std::string> tokens = config_parser::split_line(line);

      if (tokens.empty()) {
        continue;
      }

      const std::string& key = tokens[0];
      std::vector<std::string> values(tokens.begin() + 1, tokens.end());

      try {
        config_parser::parse_parameter(config, key, values, line);
      }
      catch (const std::runtime_error& e) {
        throw;
      }
      catch (const std::exception& e) {
        throw std::runtime_error("Error: Invalid config file format\nLine: \"" + line + "\"");
      }
    }

    return config;
  }

}  // namespace render

