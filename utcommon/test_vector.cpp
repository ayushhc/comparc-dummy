#include <gtest/gtest.h>
#include <cmath>

#include "vector.hpp"

TEST(test_vector, magnitude_zero) {
    render::vector vec{0.0, 0.0, 0.0};
    EXPECT_EQ(vec.magnitude(), 0.0);
}

TEST(test_vector, magnitude_positive) {
    render::vector vec{3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(vec.magnitude(), 5.0);
}

TEST(test_vector, magnitude_squared) {
    render::vector vec{3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(vec.magnitude_squared(), 25.0);
}

TEST(test_vector, normalize) {
    render::vector vec{3.0, 4.0, 0.0};
    render::vector normalized = vec.normalize();
    EXPECT_NEAR(normalized.magnitude(), 1.0, 1e-10);
    EXPECT_NEAR(normalized.get_x(), 0.6, 1e-10);
    EXPECT_NEAR(normalized.get_y(), 0.8, 1e-10);
    EXPECT_NEAR(normalized.get_z(), 0.0, 1e-10);
}

TEST(test_vector, dot_product) {
    render::vector v1{1.0, 2.0, 3.0};
    render::vector v2{4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(v1.dot(v2), 32.0);
}

TEST(test_vector, cross_product) {
    render::vector v1{1.0, 0.0, 0.0};
    render::vector v2{0.0, 1.0, 0.0};
    render::vector result = v1.cross(v2);
    EXPECT_NEAR(result.get_x(), 0.0, 1e-10);
    EXPECT_NEAR(result.get_y(), 0.0, 1e-10);
    EXPECT_NEAR(result.get_z(), 1.0, 1e-10);
}

TEST(test_vector, addition) {
    render::vector v1{1.0, 2.0, 3.0};
    render::vector v2{4.0, 5.0, 6.0};
    render::vector result = v1 + v2;
    EXPECT_DOUBLE_EQ(result.get_x(), 5.0);
    EXPECT_DOUBLE_EQ(result.get_y(), 7.0);
    EXPECT_DOUBLE_EQ(result.get_z(), 9.0);
}

TEST(test_vector, subtraction) {
    render::vector v1{4.0, 5.0, 6.0};
    render::vector v2{1.0, 2.0, 3.0};
    render::vector result = v1 - v2;
    EXPECT_DOUBLE_EQ(result.get_x(), 3.0);
    EXPECT_DOUBLE_EQ(result.get_y(), 3.0);
    EXPECT_DOUBLE_EQ(result.get_z(), 3.0);
}

TEST(test_vector, scalar_multiplication) {
    render::vector v{1.0, 2.0, 3.0};
    render::vector result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.get_x(), 2.0);
    EXPECT_DOUBLE_EQ(result.get_y(), 4.0);
    EXPECT_DOUBLE_EQ(result.get_z(), 6.0);
}

TEST(test_vector, scalar_multiplication_left) {
    render::vector v{1.0, 2.0, 3.0};
    render::vector result = 2.0 * v;
    EXPECT_DOUBLE_EQ(result.get_x(), 2.0);
    EXPECT_DOUBLE_EQ(result.get_y(), 4.0);
    EXPECT_DOUBLE_EQ(result.get_z(), 6.0);
}

TEST(test_vector, scalar_division) {
    render::vector v{2.0, 4.0, 6.0};
    render::vector result = v / 2.0;
    EXPECT_DOUBLE_EQ(result.get_x(), 1.0);
    EXPECT_DOUBLE_EQ(result.get_y(), 2.0);
    EXPECT_DOUBLE_EQ(result.get_z(), 3.0);
}

TEST(test_vector, negation) {
    render::vector v{1.0, 2.0, 3.0};
    render::vector result = -v;
    EXPECT_DOUBLE_EQ(result.get_x(), -1.0);
    EXPECT_DOUBLE_EQ(result.get_y(), -2.0);
    EXPECT_DOUBLE_EQ(result.get_z(), -3.0);
}

TEST(test_vector, compound_assignment_addition) {
    render::vector v1{1.0, 2.0, 3.0};
    render::vector v2{4.0, 5.0, 6.0};
    v1 += v2;
    EXPECT_DOUBLE_EQ(v1.get_x(), 5.0);
    EXPECT_DOUBLE_EQ(v1.get_y(), 7.0);
    EXPECT_DOUBLE_EQ(v1.get_z(), 9.0);
}

TEST(test_vector, compound_assignment_subtraction) {
    render::vector v1{4.0, 5.0, 6.0};
    render::vector v2{1.0, 2.0, 3.0};
    v1 -= v2;
    EXPECT_DOUBLE_EQ(v1.get_x(), 3.0);
    EXPECT_DOUBLE_EQ(v1.get_y(), 3.0);
    EXPECT_DOUBLE_EQ(v1.get_z(), 3.0);
}

TEST(test_vector, compound_assignment_multiplication) {
    render::vector v{1.0, 2.0, 3.0};
    v *= 2.0;
    EXPECT_DOUBLE_EQ(v.get_x(), 2.0);
    EXPECT_DOUBLE_EQ(v.get_y(), 4.0);
    EXPECT_DOUBLE_EQ(v.get_z(), 6.0);
}

TEST(test_vector, compound_assignment_division) {
    render::vector v{2.0, 4.0, 6.0};
    v /= 2.0;
    EXPECT_DOUBLE_EQ(v.get_x(), 1.0);
    EXPECT_DOUBLE_EQ(v.get_y(), 2.0);
    EXPECT_DOUBLE_EQ(v.get_z(), 3.0);
}