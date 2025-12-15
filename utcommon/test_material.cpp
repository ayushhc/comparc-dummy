#include <gtest/gtest.h>

#include "material.hpp"

TEST(test_material, matte_material) {
    render::matte_material mat{"test", 0.5, 0.6, 0.7};
    EXPECT_EQ(mat.get_type(), render::material_type::matte);
    EXPECT_EQ(mat.get_name(), "test");
    EXPECT_DOUBLE_EQ(mat.get_reflectance().get_x(), 0.5);
    EXPECT_DOUBLE_EQ(mat.get_reflectance().get_y(), 0.6);
    EXPECT_DOUBLE_EQ(mat.get_reflectance().get_z(), 0.7);
}

TEST(test_material, metal_material) {
    render::metal_material mat{"metal", 0.8, 0.9, 1.0, 2.0};
    EXPECT_EQ(mat.get_type(), render::material_type::metal);
    EXPECT_EQ(mat.get_name(), "metal");
    EXPECT_DOUBLE_EQ(mat.get_reflectance().get_x(), 0.8);
    EXPECT_DOUBLE_EQ(mat.get_reflectance().get_y(), 0.9);
    EXPECT_DOUBLE_EQ(mat.get_reflectance().get_z(), 1.0);
    EXPECT_DOUBLE_EQ(mat.get_diffusion(), 2.0);
}

TEST(test_material, refractive_material) {
    render::refractive_material mat{"glass", 1.5};
    EXPECT_EQ(mat.get_type(), render::material_type::refractive);
    EXPECT_EQ(mat.get_name(), "glass");
    EXPECT_DOUBLE_EQ(mat.get_refraction_index(), 1.5);
}

