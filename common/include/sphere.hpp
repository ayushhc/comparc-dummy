#ifndef RENDER_SPHERE_HPP
#define RENDER_SPHERE_HPP

#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <memory>
#include <optional>

namespace render {

  struct hit_info {
    double t;
    vector point;
    vector normal;
    std::shared_ptr<material> mat;
  };

  class sphere {
  public:
    sphere(const vector& center, double radius, std::shared_ptr<material> mat)
      : center_{center}, radius_{radius}, material_{mat} {
      if (radius <= 0.0) {
        throw std::invalid_argument("Sphere radius must be positive");
      }
    }

    [[nodiscard]] const vector& get_center() const { return center_; }
    [[nodiscard]] double get_radius() const { return radius_; }
    [[nodiscard]] std::shared_ptr<material> get_material() const { return material_; }

    [[nodiscard]] std::optional<hit_info> intersect(const ray& r) const {
      const vector oc = r.get_origin() - center_;
      const double a = r.get_direction().dot(r.get_direction());
      const double b = 2.0 * oc.dot(r.get_direction());
      const double c = oc.dot(oc) - radius_ * radius_;
      const double discriminant = b * b - 4.0 * a * c;

      if (discriminant < 0.0) {
        return std::nullopt;
      }

      const double sqrt_d = std::sqrt(discriminant);
      const double t1 = (-b - sqrt_d) / (2.0 * a);
      const double t2 = (-b + sqrt_d) / (2.0 * a);

      double t = (t1 > 0.0001) ? t1 : ((t2 > 0.0001) ? t2 : -1.0);

      if (t < 0.0001) {
        return std::nullopt;
      }

      const vector point = r.point_at(t);
      vector normal = (point - center_).normalize();

      return hit_info{t, point, normal, material_};
    }

  private:
    vector center_;
    double radius_;
    std::shared_ptr<material> material_;
  };

}

#endif

