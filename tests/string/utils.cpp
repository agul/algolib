#include <gtest/gtest.h>

#include "string/utils.hpp"

#include <string>

TEST(StringUtils, starts_with) {
    EXPECT_TRUE(starts_with("abc", "a"));
    EXPECT_TRUE(starts_with("abc", ""));
    EXPECT_TRUE(starts_with("abc", 'a'));
    EXPECT_TRUE(starts_with("abc", "ab"));
    EXPECT_TRUE(starts_with("abc", "abc"));
    EXPECT_TRUE(starts_with("a", "a"));
    EXPECT_TRUE(starts_with("a", ""));
    EXPECT_TRUE(starts_with("a", 'a'));
    EXPECT_TRUE(starts_with("", ""));

    EXPECT_FALSE(starts_with("abc", "c"));
    EXPECT_FALSE(starts_with("abc", 'c'));
    EXPECT_FALSE(starts_with("abc", "bc"));
    EXPECT_FALSE(starts_with("a", "b"));
    EXPECT_FALSE(starts_with("a", 'b'));
    EXPECT_FALSE(starts_with("", 'b'));
    EXPECT_FALSE(starts_with("", "abc"));
    EXPECT_FALSE(starts_with("abc", "abcabc"));
}

TEST(StringUtils, ends_with) {
    EXPECT_TRUE(ends_with("abc", "c"));
    EXPECT_TRUE(ends_with("abc", ""));
    EXPECT_TRUE(ends_with("abc", 'c'));
    EXPECT_TRUE(ends_with("abc", "bc"));
    EXPECT_TRUE(ends_with("abc", "abc"));
    EXPECT_TRUE(ends_with("a", "a"));
    EXPECT_TRUE(ends_with("a", ""));
    EXPECT_TRUE(ends_with("a", 'a'));
    EXPECT_TRUE(ends_with("", ""));

    EXPECT_FALSE(ends_with("abc", "a"));
    EXPECT_FALSE(ends_with("abc", 'a'));
    EXPECT_FALSE(ends_with("abc", "ab"));
    EXPECT_FALSE(ends_with("a", "b"));
    EXPECT_FALSE(ends_with("a", 'b'));
    EXPECT_FALSE(ends_with("", 'b'));
    EXPECT_FALSE(ends_with("", "abc"));
    EXPECT_FALSE(ends_with("abc", "abcabc"));
}
