#include "renderer.hpp"

#include "cylinder.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "sphere.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <random>

namespace render {

  renderer::renderer(const render_config& config, const scene& sc)
    : config_{config}, scene_{sc} {}

  vector renderer::get_background_color(const ray& r) const {
    const vector unit_direction = r.get_direction().normalize();
    const double m = 0.5 * (unit_direction.get_y() + 1.0);
    return config_.background_light_color * (1.0 - m) + config_.background_dark_color * m;
  }

  std::optional<hit_info> renderer::find_closest_hit(const ray& r) const {
    std::optional<hit_info> closest_hit;
    double closest_t = std::numeric_limits<double>::max();

    for (const auto& sphere : scene_.get_spheres()) {
      auto hit = sphere->intersect(r);
      if (hit && hit->t < closest_t && hit->t > 0.0001) {
        closest_t = hit->t;
        closest_hit = hit;
      }
    }

    for (const auto& cylinder : scene_.get_cylinders()) {
      auto hit = cylinder->intersect(r);
      if (hit && hit->t < closest_t && hit->t > 0.0001) {
        closest_t = hit->t;
        closest_hit = hit;
      }
    }

    return closest_hit;
  }

  vector renderer::random_in_unit_sphere(std::mt19937& material_rng) const {
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    vector p;
    do {
      p = vector{dist(material_rng), dist(material_rng), dist(material_rng)};
    } while (p.magnitude_squared() >= 1.0);
    return p;
  }

  vector renderer::random_unit_vector(std::mt19937& material_rng) const {
    return random_in_unit_sphere(material_rng).normalize();
  }

  vector renderer::reflect(const vector& v, const vector& n) const {
    return v - n * (2.0 * v.dot(n));
  }

  bool renderer::refract(const vector& v, const vector& n, double ni_over_nt, vector& refracted) const {
    const vector uv = v.normalize();
    const double dt = uv.dot(n);
    const double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    
    if (discriminant > 0.0) {
      refracted = (uv - n * dt) * ni_over_nt - n * std::sqrt(discriminant);
      return true;
    }
    return false;
  }

  double renderer::schlick(double cosine, double ref_idx) const {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
  }

  vector renderer::scatter_matte(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
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

  vector renderer::scatter_metal(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
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

  vector renderer::scatter_refractive(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
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

  vector renderer::trace_ray(const ray& r, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const {
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

