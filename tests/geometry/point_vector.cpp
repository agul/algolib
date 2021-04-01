#include <gtest/gtest.h>

#include "geometry/point_vector.hpp"

TEST(Geometry_Vector, normalize) {
    const Vector2D<int32_t> vector = {0, 1};
    const int32_t len = 10;

    EXPECT_EQ(vector.normalize(len).length(), len);
    EXPECT_EQ(vector.orthogonal().normalize(len).length(), len);
    EXPECT_EQ(vector.normalize(len).orthogonal().length(), len);
}
