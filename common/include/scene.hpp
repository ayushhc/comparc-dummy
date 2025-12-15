#ifndef RENDER_SCENE_HPP
#define RENDER_SCENE_HPP

#include "cylinder.hpp"
#include "material.hpp"
#include "sphere.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace render {

  class scene {
  public:
    void add_material(std::shared_ptr<material> mat);
    void add_sphere(std::shared_ptr<sphere> sph);
    void add_cylinder(std::shared_ptr<cylinder> cyl);

    [[nodiscard]] std::shared_ptr<material> get_material(const std::string& name) const;
    [[nodiscard]] const std::vector<std::shared_ptr<sphere>>& get_spheres() const { return spheres_; }
    [[nodiscard]] const std::vector<std::shared_ptr<cylinder>>& get_cylinders() const { return cylinders_; }

  private:
    std::map<std::string, std::shared_ptr<material>> materials_;
    std::vector<std::shared_ptr<sphere>> spheres_;
    std::vector<std::shared_ptr<cylinder>> cylinders_;
  };

  class scene_parser {
  public:
    [[nodiscard]] static scene parse(const std::string& filename);

  private:
    static std::vector<std::string> split_line(const std::string& line);
    static bool is_whitespace(const std::string& line);
  };

}

#endif

