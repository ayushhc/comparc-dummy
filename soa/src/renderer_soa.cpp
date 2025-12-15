#include "renderer_soa.hpp"

#include "material.hpp"
#include "scene_soa.hpp"
#include "sphere.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <random>

namespace render {

  renderer_soa::renderer_soa(const render_config& config, const scene_soa& sc)
    : config_{config}, scene_{sc} {}

  vector renderer_soa::get_background_color(const ray& r) const {
    const vector unit_direction = r.get_direction().normalize();
    const double m = 0.5 * (unit_direction.get_y() + 1.0);
    return config_.background_light_color * (1.0 - m) + config_.background_dark_color * m;
  }

  std::optional<hit_info> renderer_soa::find_closest_hit(const ray& r) const {
    std::optional<hit_info> closest_hit;
    double closest_t = std::numeric_limits<double>::max();

    const size_t num_spheres = scene_.get_num_spheres();
    for (size_t idx = 0; idx < num_spheres; ++idx) {
      const vector center{
        scene_.get_sphere_centers_x()[idx],
        scene_.get_sphere_centers_y()[idx],
        scene_.get_sphere_centers_z()[idx]
      };
      const double radius = scene_.get_sphere_radii()[idx];
      const auto mat = scene_.get_sphere_materials()[idx];

      const vector oc = r.get_origin() - center;
      const double a = r.get_direction().dot(r.get_direction());
      const double b = 2.0 * oc.dot(r.get_direction());
      const double c = oc.dot(oc) - radius * radius;
      const double discriminant = b * b - 4.0 * a * c;

      if (discriminant >= 0.0) {
        const double sqrt_d = std::sqrt(discriminant);
        const double t1 = (-b - sqrt_d) / (2.0 * a);
        const double t2 = (-b + sqrt_d) / (2.0 * a);
        double t = (t1 > 0.0001) ? t1 : ((t2 > 0.0001) ? t2 : -1.0);

        if (t > 0.0001 && t < closest_t) {
          const vector point = r.point_at(t);
          vector normal = (point - center).normalize();
          closest_t = t;
          closest_hit = hit_info{t, point, normal, mat};
        }
      }
    }

    const size_t num_cylinders = scene_.get_num_cylinders();
    for (size_t idx = 0; idx < num_cylinders; ++idx) {
      const vector center{
        scene_.get_cylinder_centers_x()[idx],
        scene_.get_cylinder_centers_y()[idx],
        scene_.get_cylinder_centers_z()[idx]
      };
      const double radius = scene_.get_cylinder_radii()[idx];
      const vector axis{
        scene_.get_cylinder_axes_x()[idx],
        scene_.get_cylinder_axes_y()[idx],
        scene_.get_cylinder_axes_z()[idx]
      };
      const auto mat = scene_.get_cylinder_materials()[idx];

      const vector axis_norm = axis.normalize();
      const double half_height = axis.magnitude() / 2.0;
      const vector top_center = center + axis_norm * half_height;
      const vector bottom_center = center - axis_norm * half_height;

      const vector oc = r.get_origin() - center;
      const vector dir = r.get_direction();
      
      const vector dir_perp = dir - axis_norm * dir.dot(axis_norm);
      const vector oc_perp = oc - axis_norm * oc.dot(axis_norm);

      const double a = dir_perp.dot(dir_perp);
      const double b = 2.0 * dir_perp.dot(oc_perp);
      const double c = oc_perp.dot(oc_perp) - radius * radius;
      const double discriminant = b * b - 4.0 * a * c;

      if (discriminant >= 0.0 && a > 0.0001) {
        const double sqrt_d = std::sqrt(discriminant);
        const double t1 = (-b - sqrt_d) / (2.0 * a);
        const double t2 = (-b + sqrt_d) / (2.0 * a);

        for (double t : {t1, t2}) {
          if (t > 0.0001 && t < closest_t) {
            const vector point = r.point_at(t);
            const vector to_point = point - center;
            const double projection = to_point.dot(axis_norm);
            
            if (std::abs(projection) <= half_height) {
              const vector axis_proj = axis_norm * projection;
              const vector radial = to_point - axis_proj;
              vector normal = radial.normalize();

              closest_t = t;
              closest_hit = hit_info{t, point, normal, mat};
            }
          }
        }
      }

      const double dir_dot_axis = dir.dot(axis_norm);
      if (std::abs(dir_dot_axis) > 0.0001) {
        for (const vector& cap_center : {top_center, bottom_center}) {
          const vector to_cap = cap_center - r.get_origin();
          const double t = to_cap.dot(axis_norm) / dir_dot_axis;
          
          if (t > 0.0001 && t < closest_t) {
            const vector point = r.point_at(t);
            const double dist_sq = (point - cap_center).magnitude_squared();
            
            if (dist_sq <= radius * radius) {
              const bool is_top = (cap_center.get_x() == top_center.get_x() && 
                                    cap_center.get_y() == top_center.get_y() && 
                                    cap_center.get_z() == top_center.get_z());
              const vector normal = is_top ? axis_norm : -axis_norm;
              closest_t = t;
              closest_hit = hit_info{t, point, normal, mat};
            }
          }
        }
      }
    }

    return closest_hit;
  }

  vector renderer_soa::random_in_unit_sphere(std::mt19937& material_rng) const {
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    vector p;
    do {
      p = vector{dist(material_rng), dist(material_rng), dist(material_rng)};
    } while (p.magnitude_squared() >= 1.0);
    return p;
  }

  vector renderer_soa::random_unit_vector(std::mt19937& material_rng) const {
    return random_in_unit_sphere(material_rng).normalize();
  }

  vector renderer_soa::reflect(const vector& v, const vector& n) const {
    return v - n * (2.0 * v.dot(n));
  }

  bool renderer_soa::refract(const vector& v, const vector& n, double ni_over_nt, vector& refracted) const {
    const vector uv = v.normalize();
    const double dt = uv.dot(n);
    const double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    
    if (discriminant > 0.0) {
      refracted = (uv - n * dt) * ni_over_nt - n * std::sqrt(discriminant);
      return true;
    }
    return false;
  }

  double renderer_soa::schlick(double cosine, double ref_idx) const {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
  }

  vector renderer_soa::scatter_matte(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
    const auto* mat = dynamic_cast<const matte_material*>(hit.mat.get());
    if (!mat) {
      return vector{0.0, 0.0, 0.0};
    }

    const vector target = hit.point + hit.normal + random_unit_vector(material_rng);
    const ray scattered{hit.point, (target - hit.point).normalize()};
    const vector& reflectance = mat->get_reflectance();
    const vector traced = trace_ray(scattered, depth + 1, ray_rng, material_rng);

    return vector{
      reflectance.get_x() * traced.get_x(),
      reflectance.get_y() * traced.get_y(),
      reflectance.get_z() * traced.get_z()
    };
  }

  vector renderer_soa::scatter_metal(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
    const auto* mat = dynamic_cast<const metal_material*>(hit.mat.get());
    if (!mat) {
      return vector{0.0, 0.0, 0.0};
    }

    const vector reflected = reflect(r.get_direction().normalize(), hit.normal);
    const vector fuzz = random_in_unit_sphere(material_rng) * mat->get_diffusion();
    const ray scattered{hit.point, (reflected + fuzz).normalize()};
    const vector& reflectance = mat->get_reflectance();

    if (scattered.get_direction().dot(hit.normal) > 0.0) {
      const vector traced = trace_ray(scattered, depth + 1, ray_rng, material_rng);
      return vector{
        reflectance.get_x() * traced.get_x(),
        reflectance.get_y() * traced.get_y(),
        reflectance.get_z() * traced.get_z()
      };
    }
    return vector{0.0, 0.0, 0.0};
  }

  vector renderer_soa::scatter_refractive(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
    const auto* mat = dynamic_cast<const refractive_material*>(hit.mat.get());
    if (!mat) {
      return vector{0.0, 0.0, 0.0};
    }

    vector outward_normal;
    double ni_over_nt;
    const double cosine = r.get_direction().normalize().dot(hit.normal);

    if (cosine > 0.0) {
      outward_normal = -hit.normal;
      ni_over_nt = mat->get_refraction_index();
    } else {
      outward_normal = hit.normal;
      ni_over_nt = 1.0 / mat->get_refraction_index();
    }

    vector refracted;
    const double reflect_prob = (cosine > 0.0) ? 
      schlick(cosine, mat->get_refraction_index()) : 
      schlick(-cosine, mat->get_refraction_index());

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (refract(r.get_direction().normalize(), outward_normal, ni_over_nt, refracted) && dist(material_rng) > reflect_prob) {
      const ray scattered{hit.point, refracted};
      return trace_ray(scattered, depth + 1, ray_rng, material_rng);
    } else {
      const ray scattered{hit.point, reflect(r.get_direction().normalize(), hit.normal)};
      return trace_ray(scattered, depth + 1, ray_rng, material_rng);
    }
  }

  vector renderer_soa::trace_ray(const ray& r, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
    if (depth >= config_.max_depth) {
      return vector{0.0, 0.0, 0.0};
    }

    auto hit = find_closest_hit(r);
    if (!hit) {
      return get_background_color(r);
    }

    const material_type type = hit->mat->get_type();
    
    if (type == material_type::matte) {
      return scatter_matte(r, *hit, depth, ray_rng, material_rng);
    } else if (type == material_type::metal) {
      return scatter_metal(r, *hit, depth, ray_rng, material_rng);
    } else if (type == material_type::refractive) {
      return scatter_refractive(r, *hit, depth, ray_rng, material_rng);
    }

    return vector{0.0, 0.0, 0.0};
  }

}

