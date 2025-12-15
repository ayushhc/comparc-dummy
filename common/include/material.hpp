#ifndef RENDER_MATERIAL_HPP
#define RENDER_MATERIAL_HPP

#include "vector.hpp"

#include <string>
#include <memory>

namespace render {

  enum class material_type {
    matte,
    metal,
    refractive
  };

  class material {
  public:
    material(material_type type, const std::string& name)
      : type_{type}, name_{name} {}
    virtual ~material() = default;

    [[nodiscard]] material_type get_type() const { return type_; }
    [[nodiscard]] const std::string& get_name() const { return name_; }

  private:
    material_type type_;
    std::string name_;
  };

  class matte_material : public material {
  public:
    matte_material(const std::string& name, double r, double g, double b)
      : material(material_type::matte, name), reflectance_{r, g, b} {}

    [[nodiscard]] const vector& get_reflectance() const { return reflectance_; }

  private:
    vector reflectance_;
  };

  class metal_material : public material {
  public:
    metal_material(const std::string& name, double r, double g, double b, double diffusion)
      : material(material_type::metal, name), reflectance_{r, g, b}, diffusion_{diffusion} {}

    [[nodiscard]] const vector& get_reflectance() const { return reflectance_; }
    [[nodiscard]] double get_diffusion() const { return diffusion_; }

  private:
    vector reflectance_;
    double diffusion_;
  };

  class refractive_material : public material {
  public:
    refractive_material(const std::string& name, double refraction_index)
      : material(material_type::refractive, name), refraction_index_{refraction_index} {}

    [[nodiscard]] double get_refraction_index() const { return refraction_index_; }

  private:
    double refraction_index_;
  };

}  // namespace render

#endif

