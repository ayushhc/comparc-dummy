#ifndef RENDER_RENDERER_HPP
#define RENDER_RENDERER_HPP

#include "config.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <optional>
#include <random>

namespace render {

  class renderer {
  public:
    renderer(const render_config& config, const scene& sc);

    [[nodiscard]] vector trace_ray(const ray& r, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const;
    [[nodiscard]] std::optional<hit_info> find_closest_hit(const ray& r) const;
    [[nodiscard]] vector get_background_color(const ray& r) const;

  private:
    const render_config& config_;
    const scene& scene_;

    [[nodiscard]] vector scatter_matte(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const;
    [[nodiscard]] vector scatter_metal(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const;
    [[nodiscard]] vector scatter_refractive(const ray& r, const hit_info& hit, int depth, std::mt19937& ray_rng, std::mt19937& material_rng) const;
    [[nodiscard]] vector random_in_unit_sphere(std::mt19937& material_rng) const;
    [[nodiscard]] vector random_unit_vector(std::mt19937& material_rng) const;
    [[nodiscard]] vector reflect(const vector& v, const vector& n) const;
    [[nodiscard]] bool refract(const vector& v, const vector& n, double ni_over_nt, vector& refracted) const;
    [[nodiscard]] double schlick(double cosine, double ref_idx) const;
  };

}

#endif

