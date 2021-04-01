#include <gtest/gtest.h>

#include "geometry/segment.hpp"

TEST(Geometry_Segment, center2D) {
    const Point2D<int32_t> a = {19, 94};
    const Point2D<int32_t> b = {12, 9};
    const Segment<int32_t> segment = {a, b};

    const auto segment_len = segment.length();
    const auto center = segment.center();
    EXPECT_EQ(a.as<decltype(center)>().dist(center), segment_len / 2.0);
    EXPECT_EQ(b.as<decltype(center)>().dist(center), segment_len / 2.0);
}
