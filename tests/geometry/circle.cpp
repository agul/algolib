#include <gtest/gtest.h>

#include "geometry/circle.hpp"

#include "base/constants.hpp"

TEST(Geometry_Circle, area) {
    EXPECT_EQ(Circle<int32_t>({10, 12}, 10).area(), 100 * PI);
    EXPECT_EQ(Circle<double>({12, 94}, 1.23).area(), 1.23 * 1.23 * PI);
}

TEST(Geometry_Circle, circumference) {
    EXPECT_EQ(Circle<int32_t>({10, 12}, 10).circumference(), 20 * PI);
    EXPECT_EQ(Circle<double>({12, 94}, 1.23).circumference(), 2.46 * PI);
}

TEST(Geometry_Circle, is_on_circle) {
    const Circle<int32_t> circle({10, 12}, 10);
    EXPECT_TRUE(circle.is_on_circle({0, 12}));
    EXPECT_TRUE(circle.is_on_circle({10, 2}));
    EXPECT_FALSE(circle.is_on_circle({10, 12}));
    EXPECT_FALSE(circle.is_on_circle({0, 0}));
}

TEST(Geometry_Circle, contains_point) {
    const Circle<int32_t> circle({10, 12}, 10);
    EXPECT_TRUE(circle.contains({0, 12}, false));
    EXPECT_FALSE(circle.contains({0, 12}, true));
    EXPECT_TRUE(circle.contains({10, 2}, false));
    EXPECT_FALSE(circle.contains({10, 2}, true));
    EXPECT_TRUE(circle.contains({10, 12}, true));
    EXPECT_FALSE(circle.contains({0, 0}, false));
}

TEST(Geometry_Circle, contains_segment) {
    const Circle<int32_t> circle({10, 12}, 10);
    EXPECT_TRUE(circle.contains(Segment<int32_t>{{0, 12}, {10, 2}}, false));
    EXPECT_TRUE(circle.contains(Segment<int32_t>{{0, 12}, {20, 12}}, false));
    EXPECT_TRUE(circle.contains(Segment<int32_t>{{3, 11}, {14, 14}}, true));
    EXPECT_FALSE(circle.contains(Segment<int32_t>{{0, 12}, {10, 2}}, true));
    EXPECT_FALSE(circle.contains(Segment<int32_t>{{0, 12}, {0, 0}}, true));
    EXPECT_FALSE(circle.contains(Segment<int32_t>{{1, 1}, {0, 0}}, true));
}

TEST(Geometry_Circle, contains_circle) {
    const Circle<int32_t> circle({10, 12}, 10);
    EXPECT_TRUE(circle.contains(Circle{{10, 12}, 5}, false));
    EXPECT_TRUE(circle.contains(Circle{{10, 12}, 10}, false));
    EXPECT_FALSE(circle.contains(Circle{{10, 12}, 10}, true));
    EXPECT_TRUE(circle.contains(Circle{{14, 14}, 2}, true));
    EXPECT_FALSE(circle.contains(Circle{{14, 14}, 6}, true));
    EXPECT_FALSE(circle.contains(Circle{{0, 0}, 5}, true));
    EXPECT_FALSE(circle.contains(Circle{{-100, 0}, 15}, true));
}
