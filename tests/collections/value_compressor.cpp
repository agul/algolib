#include <gtest/gtest.h>

#include "collections/value_compressor.hpp"

TEST(ValueCompressor, ctor_from_collection) {
	EXPECT_EQ(ValueCompressor<int>(std::vector<int>{4, 6, 5, 2}), std::vector<int>({4, 6, 5, 2}));
	EXPECT_EQ(ValueCompressor<int>(std::vector<int>{4, 6, 0, 5, 0, 0, 2}), std::vector<int>({4, 6, 0, 5, 2}));
	EXPECT_EQ(ValueCompressor<int>(std::vector<int>{1, 1, 1}), std::vector<int>({1}));
	EXPECT_EQ(ValueCompressor<int>(std::set<int>{}), std::vector<int>({}));
}

TEST(ValueCompressor, ctor_from_input_iterators) {
	auto a = std::vector<int>{4, 6, 5, 2};
	EXPECT_EQ(ValueCompressor<int>(a.begin(), a.end()), std::vector<int>({4, 6, 5, 2}));

	auto b = std::vector<int>{4, 6, 0, 5, 0, 0, 2};
	EXPECT_EQ(ValueCompressor<int>(b.begin(), b.end()), std::vector<int>({4, 6, 0, 5, 2}));

	auto c = std::vector<int>{1, 1, 1};
	EXPECT_EQ(ValueCompressor<int>(c.begin(), c.end()), std::vector<int>({1}));

	auto d = std::set<int>{};
	EXPECT_EQ(ValueCompressor<int>(d.begin(), d.end()), std::vector<int>({}));
}

TEST(ValueCompressor, size) {
	EXPECT_EQ(ValueCompressor<int>(std::vector<int>{4, 6, 5, 2}).size(), 4UL);
	EXPECT_EQ(ValueCompressor<int>(std::vector<int>{4, 6, 0, 5, 0, 0, 2}).size(), 5UL);
	EXPECT_EQ(ValueCompressor<int>(std::vector<int>{1, 1, 1}).size(), 1UL);
	EXPECT_EQ(ValueCompressor<int>(std::set<int>{}).size(), 0UL);
}

TEST(ValueCompressor, empty) {
	EXPECT_FALSE(ValueCompressor<int>(std::vector<int>{4, 6, 5, 2}).empty());
	EXPECT_TRUE(ValueCompressor<int>(std::set<int>{}).empty());
}

TEST(ValueCompressor, contains) {
	EXPECT_FALSE(ValueCompressor<int>(std::vector<int>{4, 6, 5, 2}).contains(7));
	EXPECT_TRUE(ValueCompressor<int>(std::vector<int>{4, 6, 0, 5, 0, 0, 2}).contains(5));
	EXPECT_FALSE(ValueCompressor<int>(std::vector<int>{4, 6, 0, 5, 0, 0, 2}).contains(-5));
	EXPECT_FALSE(ValueCompressor<int>(std::set<int>{}).contains(0));
}

TEST(ValueCompressor, additions_deletions) {
	auto check = [](const std::vector<int>& a) {
		ValueCompressor<int> value_compressor;
		std::vector<size_t> ids;
		for (const int x : a) {
			ids.emplace_back(value_compressor[x]);
		}
		std::vector<int> values;
		for (const size_t id : ids) {
			values.emplace_back(value_compressor.get_by_id(id));
		}
		EXPECT_EQ(values, a);
	};

	check(std::vector<int>{2, 3, 4, 6, 7, 100, 12});
	check(std::vector<int>{2, 2, 2, 2});
	check(std::vector<int>{0, 1, 1000000007, 1000000009, 117, 877117, 1, 1, 1, 1, 1});
	check(std::vector<int>{});
	check(std::vector<int>{-1, 1, -2, 2, 0, 0, -100});
}

TEST(ValueCompressor, ctor_from_strings_collection) {
	EXPECT_EQ(ValueCompressor<std::string>(std::vector<std::string>{"abc", "cde", "xyz", "abc"}), std::vector<std::string>({"abc", "cde", "xyz"}));
	EXPECT_EQ(ValueCompressor<std::string>(std::set<std::string>{"abc", "cde", "abc"}), std::vector<std::string>({"abc", "cde"}));
}

TEST(IntegralValueCompressor, ctor_from_input_iterators) {
	auto a = std::vector<int>{4, 6, 5, 2};
	EXPECT_EQ(IntegralValueCompressor<int>(a.begin(), a.end()), std::vector<int>({4, 6, 5, 2}));

	auto b = std::vector<int>{4, 6, 0, 5, 0, 0, 2};
	EXPECT_EQ(IntegralValueCompressor<int>(b.begin(), b.end()), std::vector<int>({4, 6, 0, 5, 2}));

	auto c = std::vector<int>{1, 1, 1};
	EXPECT_EQ(IntegralValueCompressor<int>(c.begin(), c.end()), std::vector<int>({1}));

	auto d = std::set<int>{};
	EXPECT_EQ(IntegralValueCompressor<int>(d.begin(), d.end()), std::vector<int>({}));
}

TEST(compressed, compressed) {
	EXPECT_EQ(compressed<int>(std::vector<int>{4, 6, 5, 2}), std::vector<size_t>({0, 1, 2, 3}));
	EXPECT_EQ(compressed<int>(std::vector<int>{4, 6, 0, 5, 0, 0, 2}), std::vector<size_t>({0, 1, 2, 3, 2, 2, 4}));
	EXPECT_EQ(compressed<int>(std::vector<int>{1, 1, 1}), std::vector<size_t>({0, 0, 0}));
	EXPECT_EQ(compressed<int>(std::set<int>{}), std::vector<size_t>({}));
}

TEST(compressed, strings) {
	EXPECT_EQ(compressed<std::string>(std::vector<std::string>{"abc", "xyz", "cde", "abacaba"}), std::vector<size_t>({0, 1, 2, 3}));
	EXPECT_EQ(compressed<std::string>(std::vector<std::string>{"a", "b", "c", "d", "c", "c", "c", "e"}), std::vector<size_t>({0, 1, 2, 3, 2, 2, 2, 4}));
	EXPECT_EQ(compressed<std::string>(std::vector<std::string>{"anc", "anc", "anc"}), std::vector<size_t>({0, 0, 0}));
	EXPECT_EQ(compressed<std::string>(std::set<std::string>{}), std::vector<size_t>({}));
}
