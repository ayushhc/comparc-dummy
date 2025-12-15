#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

#include "scene.hpp"

TEST(test_scene_parser, simple_scene) {
    // Create a temporary scene file
    const std::string test_file = "test_scene.txt";
    std::ofstream file(test_file);
    file << "matte: mat1 0.5 0.6 0.7\n";
    file << "sphere: 0 0 0 1.0 mat1\n";
    file.close();

    auto scene = render::scene_parser::parse(test_file);
    EXPECT_EQ(scene.get_spheres().size(), 1);
    EXPECT_EQ(scene.get_spheres()[0]->get_radius(), 1.0);

    // Cleanup
    std::remove(test_file.c_str());
}

TEST(test_scene_parser, material_not_found) {
    const std::string test_file = "test_scene_error.txt";
    std::ofstream file(test_file);
    file << "sphere: 0 0 0 1.0 nonexistent\n";
    file.close();

    EXPECT_THROW(render::scene_parser::parse(test_file), std::runtime_error);

    std::remove(test_file.c_str());
}

TEST(test_scene_parser, invalid_sphere_radius) {
    const std::string test_file = "test_scene_error2.txt";
    std::ofstream file(test_file);
    file << "matte: mat1 0.5 0.5 0.5\n";
    file << "sphere: 0 0 0 -1.0 mat1\n";
    file.close();

    EXPECT_THROW(render::scene_parser::parse(test_file), std::runtime_error);

    std::remove(test_file.c_str());
}

TEST(test_scene_parser, duplicate_material) {
    const std::string test_file = "test_scene_error3.txt";
    std::ofstream file(test_file);
    file << "matte: mat1 0.5 0.5 0.5\n";
    file << "matte: mat1 0.6 0.6 0.6\n";
    file.close();

    EXPECT_THROW(render::scene_parser::parse(test_file), std::runtime_error);

    std::remove(test_file.c_str());
}

TEST(test_scene_parser, unknown_entity) {
    const std::string test_file = "test_scene_error4.txt";
    std::ofstream file(test_file);
    file << "unknown: test\n";
    file.close();

    EXPECT_THROW(render::scene_parser::parse(test_file), std::runtime_error);

    std::remove(test_file.c_str());
}

TEST(test_scene, add_materials_and_objects) {
    render::scene scene;
    
    auto mat1 = std::make_shared<render::matte_material>("mat1", 0.5, 0.5, 0.5);
    scene.add_material(mat1);
    
    EXPECT_NE(scene.get_material("mat1"), nullptr);
    EXPECT_EQ(scene.get_material("nonexistent"), nullptr);
    
    auto sph = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, mat1);
    scene.add_sphere(sph);
    
    EXPECT_EQ(scene.get_spheres().size(), 1);
}

