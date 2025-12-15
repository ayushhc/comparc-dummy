#ifndef RENDER_SCENE_SOA_HPP
#define RENDER_SCENE_SOA_HPP

#include "material.hpp"
#include "vector.hpp"

#include <memory>
#include <string>
#include <vector>

namespace render {

  class scene_soa {
  public:
    void add_sphere(const vector& center, double radius, std::shared_ptr<material> mat);
    void add_cylinder(const vector& center, double radius, const vector& axis, std::shared_ptr<material> mat);

    [[nodiscard]] size_t get_num_spheres() const { return sphere_centers_x_.size(); }
    [[nodiscard]] size_t get_num_cylinders() const { return cylinder_centers_x_.size(); }

    [[nodiscard]] const std::vector<double>& get_sphere_centers_x() const { return sphere_centers_x_; }
    [[nodiscard]] const std::vector<double>& get_sphere_centers_y() const { return sphere_centers_y_; }
    [[nodiscard]] const std::vector<double>& get_sphere_centers_z() const { return sphere_centers_z_; }
    [[nodiscard]] const std::vector<double>& get_sphere_radii() const { return sphere_radii_; }
    [[nodiscard]] const std::vector<std::shared_ptr<material>>& get_sphere_materials() const { return sphere_materials_; }

    [[nodiscard]] const std::vector<double>& get_cylinder_centers_x() const { return cylinder_centers_x_; }
    [[nodiscard]] const std::vector<double>& get_cylinder_centers_y() const { return cylinder_centers_y_; }
    [[nodiscard]] const std::vector<double>& get_cylinder_centers_z() const { return cylinder_centers_z_; }
    [[nodiscard]] const std::vector<double>& get_cylinder_radii() const { return cylinder_radii_; }
    [[nodiscard]] const std::vector<double>& get_cylinder_axes_x() const { return cylinder_axes_x_; }
    [[nodiscard]] const std::vector<double>& get_cylinder_axes_y() const { return cylinder_axes_y_; }
    [[nodiscard]] const std::vector<double>& get_cylinder_axes_z() const { return cylinder_axes_z_; }
    [[nodiscard]] const std::vector<std::shared_ptr<material>>& get_cylinder_materials() const { return cylinder_materials_; }

  private:
    std::vector<double> sphere_centers_x_;
    std::vector<double> sphere_centers_y_;
    std::vector<double> sphere_centers_z_;
    std::vector<double> sphere_radii_;
    std::vector<std::shared_ptr<material>> sphere_materials_;

    std::vector<double> cylinder_centers_x_;
    std::vector<double> cylinder_centers_y_;
    std::vector<double> cylinder_centers_z_;
    std::vector<double> cylinder_radii_;
    std::vector<double> cylinder_axes_x_;
    std::vector<double> cylinder_axes_y_;
    std::vector<double> cylinder_axes_z_;
    std::vector<std::shared_ptr<material>> cylinder_materials_;
  };

}

#endif

