#include <iostream>
#include <random>
#include <vector>

#include "camera.hpp"
#include "config.hpp"
#include "cylinder.hpp"
#include "renderer_soa.hpp"
#include "renderer_utils.hpp"
#include "scene.hpp"
#include "scene_soa.hpp"
#include "sphere.hpp"
#include "vector.hpp"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <config_file> <scene_file> <output_file>\n";
    return 1;
  }

  const std::string config_file = argv[1];
  const std::string scene_file = argv[2];
  const std::string output_file = argv[3];

  try {
    const auto config = render::config_parser::parse(config_file);
    const auto scene_aos = render::scene_parser::parse(scene_file);

    render::scene_soa scene_soa;
    for (const auto& sphere : scene_aos.get_spheres()) {
      scene_soa.add_sphere(sphere->get_center(), sphere->get_radius(), sphere->get_material());
    }
    for (const auto& cylinder : scene_aos.get_cylinders()) {
      scene_soa.add_cylinder(cylinder->get_center(), cylinder->get_radius(), cylinder->get_axis(), cylinder->get_material());
    }

    const render::camera cam{config};
    const render::renderer_soa renderer{config, scene_soa};

    const int width = cam.get_image_width();
    const int height = cam.get_image_height();

    std::mt19937 ray_rng(config.ray_rng_seed);
    std::mt19937 material_rng(config.material_rng_seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<std::vector<render::vector>> image(static_cast<size_t>(width));
    for (int i = 0; i < width; ++i) {
      image[i].resize(static_cast<size_t>(height));
    }

    std::cout << "Rendering " << width << "x" << height << " image with " << config.samples_per_pixel << " samples per pixel (SOA)...\n";

    for (int j = 0; j < height; ++j) {
      for (int i = 0; i < width; ++i) {
        render::vector color{0.0, 0.0, 0.0};

        for (int s = 0; s < config.samples_per_pixel; ++s) {
          const double u = (static_cast<double>(i) + dist(ray_rng)) / static_cast<double>(width);
          const double v = (static_cast<double>(j) + dist(ray_rng)) / static_cast<double>(height);
          const render::ray r = cam.get_ray(u, v);
          color = color + renderer.trace_ray(r, 0, ray_rng, material_rng);
        }

        color = color / static_cast<double>(config.samples_per_pixel);
        color = render::gamma_correct(color, config.gamma);
        color = render::clamp_color(color);
        image[i][j] = color;
      }

      if ((j + 1) % 50 == 0) {
        std::cout << "Progress: " << (j + 1) << "/" << height << " lines\n";
      }
    }

    render::write_ppm(output_file, image, width, height);
    std::cout << "Rendering complete. Output written to " << output_file << "\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
