#ifndef RENDER_CYLINDER_HPP
#define RENDER_CYLINDER_HPP

#include "material.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <cmath>
#include <memory>
#include <optional>

namespace render {

  class cylinder {
  public:
    cylinder(const vector& center, double radius, const vector& axis, std::shared_ptr<material> mat)
      : center_{center}, radius_{radius}, axis_{axis}, material_{mat} {
      if (radius <= 0.0) {
        throw std::invalid_argument("Cylinder radius must be positive");
      }
    }

    [[nodiscard]] const vector& get_center() const { return center_; }
    [[nodiscard]] double get_radius() const { return radius_; }
    [[nodiscard]] const vector& get_axis() const { return axis_; }
    [[nodiscard]] std::shared_ptr<material> get_material() const { return material_; }

    [[nodiscard]] std::optional<hit_info> intersect(const ray& r) const {
      const vector axis_norm = axis_.normalize();
      const double half_height = axis_.magnitude() / 2.0;
      const vector top_center = center_ + axis_norm * half_height;
      const vector bottom_center = center_ - axis_norm * half_height;

      const vector oc = r.get_origin() - center_;
      const vector dir = r.get_direction();
      
      // Project ray direction and origin onto plane perpendicular to axis
      const vector dir_perp = dir - axis_norm * dir.dot(axis_norm);
      const vector oc_perp = oc - axis_norm * oc.dot(axis_norm);

      // Solve quadratic for cylinder side intersection
      const double a = dir_perp.dot(dir_perp);
      const double b = 2.0 * dir_perp.dot(oc_perp);
      const double c = oc_perp.dot(oc_perp) - radius_ * radius_;
      const double discriminant = b * b - 4.0 * a * c;

      std::optional<hit_info> best_hit;

      if (discriminant >= 0.0 && a > 0.0001) {
        const double sqrt_d = std::sqrt(discriminant);
        const double t1 = (-b - sqrt_d) / (2.0 * a);
        const double t2 = (-b + sqrt_d) / (2.0 * a);

        for (double t : {t1, t2}) {
          if (t > 0.0001) {
            const vector point = r.point_at(t);
            const vector to_point = point - center_;
            const double projection = to_point.dot(axis_norm);
            
            // Check if point is within cylinder height
            if (std::abs(projection) <= half_height) {
              const vector to_center = point - center_;
              const vector axis_proj = axis_norm * projection;
              const vector radial = to_center - axis_proj;
              vector normal = radial.normalize();

              if (!best_hit || t < best_hit->t) {
                best_hit = hit_info{t, point, normal, material_};
              }
            }
          }
        }
      }

      // Check end caps
      const double dir_dot_axis = dir.dot(axis_norm);
      if (std::abs(dir_dot_axis) > 0.0001) {
        for (const vector& cap_center : {top_center, bottom_center}) {
          const vector to_cap = cap_center - r.get_origin();
          const double t = to_cap.dot(axis_norm) / dir_dot_axis;
          
          if (t > 0.0001) {
            const vector point = r.point_at(t);
            const double dist_sq = (point - cap_center).magnitude_squared();
            
            if (dist_sq <= radius_ * radius_) {
              const bool is_top = (cap_center.get_x() == top_center.get_x() && 
                                    cap_center.get_y() == top_center.get_y() && 
                                    cap_center.get_z() == top_center.get_z());
              const vector normal = is_top ? axis_norm : -axis_norm;
              
              if (!best_hit || t < best_hit->t) {
                best_hit = hit_info{t, point, normal, material_};
              }
            }
          }
        }
      }

      return best_hit;
    }

  private:
    vector center_;
    double radius_;
    vector axis_;
    std::shared_ptr<material> material_;
  };

}  // namespace render

#endif

