#include <gtest/gtest.h>

#include "data_structures/fenwick_tree.hpp"

TEST(FenwickTreeSum, build_from_vector) {
	auto check = [](std::initializer_list<int>&& init_list) {
		const std::vector<int> a(init_list);

		FenwickTreeSum<int> fenwick_tree(a.size());
		for (size_t i = 0; i < a.size(); ++i) {
			fenwick_tree.inc(i, a[i]);
		}

		EXPECT_EQ(fenwick_tree, FenwickTreeSum<int>(a));
		EXPECT_EQ(fenwick_tree, FenwickTreeSum<int>(init_list));
	};

	check({1, 2, 3, 7, 117, 54});
	check({100, 2000, 30000, 9000});
	check({1});
	check({0});
	check({1, 1, 1, 1});
	check({0, 0, 0, 0, 0});
	check({117, 0, 0, 0});
	check({0, 0, 0, 0, 54});
	check({});
}

TEST(FenwickTreeSum, fast_range_query) {
	auto check = [](std::initializer_list<int>&& init_list) {
		const FenwickTreeSum<int> fenwick_tree(init_list);
		auto slow_query = [&fenwick_tree](const size_t left, const size_t right) {
			if (right < left) {
				return 0;
			}
			if (left == 0) {
				return fenwick_tree.query(right);
			}
			return fenwick_tree.query(right) - fenwick_tree.query(left - 1);
		};

		for (size_t i = 0; i < fenwick_tree.size(); ++i) {
			for (size_t j = 0; j < fenwick_tree.size(); ++j) {
				EXPECT_EQ(slow_query(i, j), fenwick_tree.query(i, j));
			}
		}
	};

	check({1, 2, 3, 7, 117, 54});
	check({100, 2000, 30000, 9000});
	check({1});
	check({0});
	check({1, 1, 1, 1});
	check({0, 0, 0, 0, 0});
	check({117, 0, 0, 0});
	check({0, 0, 0, 0, 54});
	check({});
}
