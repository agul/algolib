#include <gtest/gtest.h>

#include "geometry/segment.hpp"

TEST(Geometry_Segment, center) {
    const Point2D<int32_t> a = {19, 94};
    const Point2D<int32_t> b = {12, 9};
    const Segment<int32_t> segment = {a, b};

    const auto segment_len = segment.length();
    const auto center = segment.center();
    EXPECT_EQ(a.as<decltype(center)>().dist(center), segment_len / 2.0);
    EXPECT_EQ(b.as<decltype(center)>().dist(center), segment_len / 2.0);
}

TEST(Geometry_Segment, contains_point) {
    const Point2D<double> a = {1, 1};
    const Point2D<double> b = {5, 9};
    const Segment<double> segment = {a, b};

    EXPECT_TRUE(segment.contains({1, 1}, false));
    EXPECT_FALSE(segment.contains({1, 1}, true));
    EXPECT_TRUE(segment.contains({5, 9}, false));
    EXPECT_FALSE(segment.contains({5, 9}, true));
    EXPECT_TRUE(segment.contains({2, 3}, false));
    EXPECT_TRUE(segment.contains({2, 3}, true));
    EXPECT_TRUE(segment.contains({2, 3}, false));
    EXPECT_TRUE(segment.contains({2.5, 4}, true));
    EXPECT_TRUE(segment.contains({2.25, 3.5}, false));
    EXPECT_FALSE(segment.contains({2.25, 3.25}, false));
    EXPECT_FALSE(segment.contains({0, 0}, false));
}

TEST(Geometry_Segment, starts_from) {
    const Point2D<double> a = {1, 1};
    const Point2D<double> b = {5, 9};
    const Segment<double> segment = {a, b};

    EXPECT_TRUE(segment.starts_from({1, 1}));
    EXPECT_TRUE(segment.starts_from({5, 9}));
    EXPECT_FALSE(segment.starts_from({2, 3}));
    EXPECT_FALSE(segment.starts_from({2.5, 4}));
    EXPECT_FALSE(segment.starts_from({2.25, 3.5}));
    EXPECT_FALSE(segment.starts_from({0, 0}));
}
