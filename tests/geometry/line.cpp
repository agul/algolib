#include <gtest/gtest.h>

#include "geometry/line.hpp"

TEST(Geometry_Line, get_parallel) {
    const Point2D<int32_t> a = {19, 94};
    const Point2D<int32_t> b = {12, 9};

    const int32_t dist = 10;
    const Line<int32_t> line = {a, b};
    const auto parallel_line = line.get_parallel(dist);

    EXPECT_TRUE(line.as<decltype(parallel_line)>().is_parallel(parallel_line));
    EXPECT_NE(line.as<decltype(parallel_line)>(), parallel_line);
    EXPECT_EQ(line.as<decltype(parallel_line)>().dist(parallel_line.point_a()), dist);
    EXPECT_EQ(line.as<decltype(parallel_line)>().dist(parallel_line.point_b()), dist);
}

TEST(Geometry_Line, is_parallel) {
    const Point2D<int32_t> a = {0, 0};
    const Point2D<int32_t> b = {1, 0};
    const Line<int32_t> line = {a, b};

    const auto parallel_line = line.get_parallel(10);
    EXPECT_TRUE(line.as<decltype(parallel_line)>().is_parallel(parallel_line));

    EXPECT_FALSE(line.is_parallel(Line<int32_t>({1, 1}, {1, 2})));
    EXPECT_TRUE(line.is_parallel(Line<int32_t>({9, 100}, {18, 100})));
}

TEST(Geometry_Line, is_equal) {
    const Point2D<int32_t> a = {0, 0};
    const Point2D<int32_t> b = {1, 1};
    const Line<int32_t> line = {a, b};

    const auto parallel_line = line.get_parallel(10);
    EXPECT_FALSE(line.as<decltype(parallel_line)>().is_equal(parallel_line));

    EXPECT_FALSE(line.is_equal(Line<int32_t>({1, 1}, {1, 2})));
    EXPECT_TRUE(line.is_equal(Line<int32_t>({9, 9}, {18, 18})));
    EXPECT_FALSE(line.is_equal(line.perpendicular({12, 94})));
    EXPECT_TRUE(line.is_equal(line.perpendicular({12, 94}).perpendicular({12, 12})));
}
