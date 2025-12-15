#include "scene_soa.hpp"

namespace render {

  void scene_soa::add_sphere(const vector& center, double radius, std::shared_ptr<material> mat) {
    sphere_centers_x_.push_back(center.get_x());
    sphere_centers_y_.push_back(center.get_y());
    sphere_centers_z_.push_back(center.get_z());
    sphere_radii_.push_back(radius);
    sphere_materials_.push_back(mat);
  }

  void scene_soa::add_cylinder(const vector& center, double radius, const vector& axis, std::shared_ptr<material> mat) {
    cylinder_centers_x_.push_back(center.get_x());
    cylinder_centers_y_.push_back(center.get_y());
    cylinder_centers_z_.push_back(center.get_z());
    cylinder_radii_.push_back(radius);
    cylinder_axes_x_.push_back(axis.get_x());
    cylinder_axes_y_.push_back(axis.get_y());
    cylinder_axes_z_.push_back(axis.get_z());
    cylinder_materials_.push_back(mat);
  }

}  // namespace render

