#include <gtest/gtest.h>

#include "range/ranges.hpp"
#include "range/reversed_range.hpp"

#include <vector>


TEST(Ranges, Range) {
	const auto a = range(5);
	EXPECT_EQ(std::vector<int>(a.begin(), a.end()), std::vector<int>({0, 1, 2, 3, 4}));

	const auto b = range(0);
	EXPECT_EQ(std::vector<int>(b.begin(), b.end()), std::vector<int>({}));

	const auto c = range(10, 13);
	EXPECT_EQ(std::vector<int>(c.begin(), c.end()), std::vector<int>({10, 11, 12}));

	const auto d = range(-5, 2);
	EXPECT_EQ(std::vector<int>(d.begin(), d.end()), std::vector<int>({-5, -4, -3, -2, -1, 0, 1}));
}

TEST(Ranges, InclusiveRange) {
	const auto a = inclusiveRange(4);
	EXPECT_EQ(std::vector<int>(a.begin(), a.end()), std::vector<int>({0, 1, 2, 3, 4}));

	const auto b = inclusiveRange(0);
	EXPECT_EQ(std::vector<int>(b.begin(), b.end()), std::vector<int>({0}));

	const auto c = inclusiveRange(10, 13);
	EXPECT_EQ(std::vector<int>(c.begin(), c.end()), std::vector<int>({10, 11, 12, 13}));

	const auto d = inclusiveRange(-5, 2);
	EXPECT_EQ(std::vector<int>(d.begin(), d.end()), std::vector<int>({-5, -4, -3, -2, -1, 0, 1, 2}));
}

TEST(Ranges, Downrange) {
	const auto a = downrange(5);
	EXPECT_EQ(std::vector<int>(a.begin(), a.end()), std::vector<int>({4, 3, 2, 1, 0}));

	const auto b = downrange(0);
	EXPECT_EQ(std::vector<int>(b.begin(), b.end()), std::vector<int>({}));

	const auto c = downrange(13, 10);
	EXPECT_EQ(std::vector<int>(c.begin(), c.end()), std::vector<int>({12, 11, 10}));

	const auto d = downrange(2, -5);
	EXPECT_EQ(std::vector<int>(d.begin(), d.end()), std::vector<int>({1, 0, -1, -2, -3, -4, -5}));
}

TEST(Ranges, InclusiveDownrange) {
	const auto a = inclusiveDownrange(5);
	EXPECT_EQ(std::vector<int>(a.begin(), a.end()), std::vector<int>({5, 4, 3, 2, 1, 0}));

	const auto b = inclusiveDownrange(0);
	EXPECT_EQ(std::vector<int>(b.begin(), b.end()), std::vector<int>({0}));

	const auto c = inclusiveDownrange(13, 10);
	EXPECT_EQ(std::vector<int>(c.begin(), c.end()), std::vector<int>({13, 12, 11, 10}));

	const auto d = inclusiveDownrange(2, -5);
	EXPECT_EQ(std::vector<int>(d.begin(), d.end()), std::vector<int>({2, 1, 0, -1, -2, -3, -4, -5}));
}

TEST(Ranges, Reversed) {
	const auto a = reversed(range(3));
	EXPECT_EQ(std::vector<int>(a.begin(), a.end()), std::vector<int>({2, 1, 0}));

	const auto b = reversed(inclusiveRange(0));
	EXPECT_EQ(std::vector<int>(b.begin(), b.end()), std::vector<int>({0}));

	const std::vector<int> tmp = {5, 4, 3, 2, 1};
	const auto d = reversed(tmp);
	EXPECT_EQ(std::vector<int>(d.begin(), d.end()), std::vector<int>({1, 2, 3, 4, 5}));
}
