#include "scene.hpp"

#include "cylinder.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace render {

  void scene::add_material(std::shared_ptr<material> mat) {
    const std::string& name = mat->get_name();
    if (materials_.find(name) != materials_.end()) {
      throw std::runtime_error("Material with name [" + name + "] already exists");
    }
    materials_[name] = mat;
  }

  void scene::add_sphere(std::shared_ptr<sphere> sph) {
    spheres_.push_back(sph);
  }

  void scene::add_cylinder(std::shared_ptr<cylinder> cyl) {
    cylinders_.push_back(cyl);
  }

  std::shared_ptr<material> scene::get_material(const std::string& name) const {
    auto it = materials_.find(name);
    if (it == materials_.end()) {
      return nullptr;
    }
    return it->second;
  }

  std::vector<std::string> scene_parser::split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
      tokens.push_back(token);
    }
    
    return tokens;
  }

  bool scene_parser::is_whitespace(const std::string& line) {
    for (char c : line) {
      if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
        return false;
      }
    }
    return true;
  }

  scene scene_parser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Error: Could not open scene file: " + filename);
    }

    scene sc;
    std::string line;
    int line_number = 0;

    while (std::getline(file, line)) {
      line_number++;

      while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
        line.pop_back();
      }

      if (is_whitespace(line)) {
        continue;
      }

      std::vector<std::string> tokens = split_line(line);

      if (tokens.empty()) {
        continue;
      }

      try {
        const std::string& first = tokens[0];

        if (first == "matte:") {
          if (tokens.size() != 5) {
            throw std::runtime_error("Error: Invalid matte material parameters\nLine: \"" + line + "\"");
          }
          const std::string& name = tokens[1];
          const double r = std::stod(tokens[2]);
          const double g = std::stod(tokens[3]);
          const double b = std::stod(tokens[4]);
          
          if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
            throw std::runtime_error("Error: Invalid matte material parameters\nLine: \"" + line + "\"");
          }
          
          sc.add_material(std::make_shared<matte_material>(name, r, g, b));
        }
        else if (first == "metal:") {
          if (tokens.size() != 6) {
            throw std::runtime_error("Error: Invalid metal material parameters\nLine: \"" + line + "\"");
          }
          const std::string& name = tokens[1];
          const double r = std::stod(tokens[2]);
          const double g = std::stod(tokens[3]);
          const double b = std::stod(tokens[4]);
          const double diffusion = std::stod(tokens[5]);
          
          if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
            throw std::runtime_error("Error: Invalid metal material parameters\nLine: \"" + line + "\"");
          }
          
          sc.add_material(std::make_shared<metal_material>(name, r, g, b, diffusion));
        }
        else if (first == "refractive:") {
          if (tokens.size() != 3) {
            throw std::runtime_error("Error: Invalid refractive material parameters\nLine: \"" + line + "\"");
          }
          const std::string& name = tokens[1];
          const double refraction_index = std::stod(tokens[2]);
          
          if (refraction_index <= 0.0) {
            throw std::runtime_error("Error: Invalid refractive material parameters\nLine: \"" + line + "\"");
          }
          
          sc.add_material(std::make_shared<refractive_material>(name, refraction_index));
        }
        else if (first == "sphere:") {
          if (tokens.size() < 6) {
            throw std::runtime_error("Error: Invalid sphere parameters\nLine: \"" + line + "\"");
          }
          if (tokens.size() > 6) {
            throw std::runtime_error("Error: Extra data after configuration value for key: [sphere:]\nExtra: \"" + tokens[6] + "\"\nLine: \"" + line + "\"");
          }
          
          const double cx = std::stod(tokens[1]);
          const double cy = std::stod(tokens[2]);
          const double cz = std::stod(tokens[3]);
          const double radius = std::stod(tokens[4]);
          const std::string& mat_name = tokens[5];
          
          if (radius <= 0.0) {
            throw std::runtime_error("Error: Invalid sphere parameters\nLine: \"" + line + "\"");
          }
          
          auto mat = sc.get_material(mat_name);
          if (!mat) {
            throw std::runtime_error("Error: Material not found: [" + mat_name + "]\nLine: \"" + line + "\"");
          }
          
          sc.add_sphere(std::make_shared<sphere>(vector{cx, cy, cz}, radius, mat));
        }
        else if (first == "cylinder:") {
          if (tokens.size() < 9) {
            throw std::runtime_error("Error: Invalid cylinder parameters\nLine: \"" + line + "\"");
          }
          if (tokens.size() > 9) {
            throw std::runtime_error("Error: Extra data after configuration value for key: [cylinder:]\nExtra: \"" + tokens[9] + "\"\nLine: \"" + line + "\"");
          }
          
          const double cx = std::stod(tokens[1]);
          const double cy = std::stod(tokens[2]);
          const double cz = std::stod(tokens[3]);
          const double radius = std::stod(tokens[4]);
          const double ax = std::stod(tokens[5]);
          const double ay = std::stod(tokens[6]);
          const double az = std::stod(tokens[7]);
          const std::string& mat_name = tokens[8];
          
          if (radius <= 0.0) {
            throw std::runtime_error("Error: Invalid cylinder parameters\nLine: \"" + line + "\"");
          }
          
          auto mat = sc.get_material(mat_name);
          if (!mat) {
            throw std::runtime_error("Error: Material not found: [" + mat_name + "]\nLine: \"" + line + "\"");
          }
          
          sc.add_cylinder(std::make_shared<cylinder>(vector{cx, cy, cz}, radius, vector{ax, ay, az}, mat));
        }
        else {
          throw std::runtime_error("Error: Unknown scene entity: " + first);
        }
      }
      catch (const std::runtime_error& e) {
        throw;
      }
      catch (const std::exception& e) {
        throw std::runtime_error("Error: Invalid scene file format\nLine: \"" + line + "\"");
      }
    }

    return sc;
  }

}

