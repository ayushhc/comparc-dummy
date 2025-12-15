#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

#include "config.hpp"

TEST(test_config_parser, basic_config) {
    const std::string test_file = "test_config.txt";
    std::ofstream file(test_file);
    file << "image_width: 800\n";
    file << "gamma: 2.2\n";
    file << "samples_per_pixel: 100\n";
    file.close();

    auto config = render::config_parser::parse(test_file);
    EXPECT_EQ(config.image_width, 800);
    EXPECT_DOUBLE_EQ(config.gamma, 2.2);
    EXPECT_EQ(config.samples_per_pixel, 100);

    std::remove(test_file.c_str());
}

TEST(test_config_parser, default_values) {
    render::render_config config;
    EXPECT_EQ(config.image_width, 800);
    EXPECT_DOUBLE_EQ(config.gamma, 2.2);
    EXPECT_EQ(config.samples_per_pixel, 100);
    EXPECT_EQ(config.max_depth, 50);
}

TEST(test_config_parser, camera_parameters) {
    const std::string test_file = "test_config2.txt";
    std::ofstream file(test_file);
    file << "camera_position: 1.0 2.0 3.0\n";
    file << "camera_target: 0.0 0.0 0.0\n";
    file << "camera_north: 0.0 1.0 0.0\n";
    file << "field_of_view: 45.0\n";
    file.close();

    auto config = render::config_parser::parse(test_file);
    EXPECT_DOUBLE_EQ(config.camera_position.get_x(), 1.0);
    EXPECT_DOUBLE_EQ(config.camera_position.get_y(), 2.0);
    EXPECT_DOUBLE_EQ(config.camera_position.get_z(), 3.0);
    EXPECT_DOUBLE_EQ(config.camera_target.get_x(), 0.0);
    EXPECT_DOUBLE_EQ(config.field_of_view, 45.0);

    std::remove(test_file.c_str());
}

TEST(test_config_parser, unknown_parameter) {
    const std::string test_file = "test_config_error.txt";
    std::ofstream file(test_file);
    file << "unknown_param: 123\n";
    file.close();

    EXPECT_THROW(render::config_parser::parse(test_file), std::runtime_error);

    std::remove(test_file.c_str());
}

