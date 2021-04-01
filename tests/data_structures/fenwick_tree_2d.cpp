#include <gtest/gtest.h>

#include "cpplib/data_structures/fenwick_tree/fenwick_tree_2d.hpp"
#include "base/helpers.hpp"
#include "range/ranges.hpp"
#include "maths/random.hpp"

TEST(FenwickTreeSum2D, check_range_query) {
    const int kMaxn = 50;
    const int kMaxValue = 10;

    auto a = make_vector<int>(kMaxn, kMaxn, 0);
    for (auto& row : a) {
        for (auto& col : row) {
            col = Random::get(1, kMaxValue);
        }
    }

    FenwickTreeSum2D<int> tree(kMaxn, kMaxn);
    for (int i : range(kMaxn)) {
        for (int j : range(kMaxn)) {
            tree.inc(i, j, a[i][j]);
        }
    }

    auto pref = make_vector<int>(kMaxn + 1, kMaxn + 1, 0);
    for (int i : range(kMaxn)) {
        for (int j : range(kMaxn)) {
            pref[i + 1][j + 1] = -pref[i][j] + pref[i + 1][j] + pref[i][j + 1] + a[i][j];
        }
    }

    for (int x1 : range(kMaxn)) {
        for (int y1 : range(kMaxn)) {
            for (int x2 : range(x1, kMaxn)) {
                for (int y2 : range(y1, kMaxn)) {
                    const int pref_sum_query = pref[x2 + 1][y2 + 1] + pref[x1][y1] - pref[x2 + 1][y1] - pref[x1][y2 + 1];
                    const int tree_query = tree.query(x1, y1, x2, y2);
                    EXPECT_EQ(pref_sum_query, tree_query);
                }
            }
        }
    }
}
