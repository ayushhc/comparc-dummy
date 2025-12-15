#include <gtest/gtest.h>

#include "material.hpp"
#include "ray.hpp"
#include "sphere.hpp"

TEST(test_sphere, creation) {
    auto mat = std::make_shared<render::matte_material>("mat1", 0.5, 0.5, 0.5);
    render::sphere sph{render::vector{0.0, 0.0, 0.0}, 1.0, mat};
    
    EXPECT_DOUBLE_EQ(sph.get_center().get_x(), 0.0);
    EXPECT_DOUBLE_EQ(sph.get_center().get_y(), 0.0);
    EXPECT_DOUBLE_EQ(sph.get_center().get_z(), 0.0);
    EXPECT_DOUBLE_EQ(sph.get_radius(), 1.0);
}

TEST(test_sphere, invalid_radius) {
    auto mat = std::make_shared<render::matte_material>("mat1", 0.5, 0.5, 0.5);
    EXPECT_THROW(render::sphere{render::vector{0.0, 0.0, 0.0}, -1.0, mat}, std::invalid_argument);
    EXPECT_THROW(render::sphere{render::vector{0.0, 0.0, 0.0}, 0.0, mat}, std::invalid_argument);
}

TEST(test_sphere, intersection_hit) {
    auto mat = std::make_shared<render::matte_material>("mat1", 0.5, 0.5, 0.5);
    render::sphere sph{render::vector{0.0, 0.0, 0.0}, 1.0, mat};
    
    render::ray r{render::vector{0.0, 0.0, 5.0}, render::vector{0.0, 0.0, -1.0}};
    auto hit = sph.intersect(r);
    
    ASSERT_TRUE(hit.has_value());
    EXPECT_GT(hit->t, 0.0);
    EXPECT_NEAR(hit->point.get_z(), 1.0, 1e-6);
}

TEST(test_sphere, intersection_miss) {
    auto mat = std::make_shared<render::matte_material>("mat1", 0.5, 0.5, 0.5);
    render::sphere sph{render::vector{0.0, 0.0, 0.0}, 1.0, mat};
    
    render::ray r{render::vector{0.0, 0.0, 5.0}, render::vector{1.0, 0.0, 0.0}};
    auto hit = sph.intersect(r);
    
    EXPECT_FALSE(hit.has_value());
}

