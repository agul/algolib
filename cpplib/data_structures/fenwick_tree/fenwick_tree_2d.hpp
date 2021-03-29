#pragma once
#include <cstdint>
#include <vector>

#include "fenwick_tree.hpp"

template<typename T>
class FenwickTreeSum2D {
public:
    using value_type = T;
    using size_type = std::size_t;
    using fenwick_tree_1d_type = FenwickTreeSum<value_type>;
    using container_type = std::vector<fenwick_tree_1d_type>;

    explicit FenwickTreeSum2D(const size_type n, const size_type m) : data_(n, fenwick_tree_1d_type(m)) {}

    void clear() {
        for (auto& row : data_) {
            row.clear();
        }
    }

    size_type rows() const {
        return data_.size();
    }

    size_type cols() const {
        return data_.front().size();
    }

    container_type& data() {
        return data_;
    }

    const container_type& data() const {
        return data_;
    }

    void inc(const size_type x, const size_type y, const value_type delta) {
        for (size_type v = x; v < data_.size(); v = (v | (v + 1))) {
            data_[v].inc(y, delta);
        }
    }

    value_type query(const size_type x, const size_type y) const
    // returns sum for range {0, 0}..{x, y}
    {
        value_type res = 0;
        for (int32_t v = x; v >= 0; v = (v & (v + 1)) - 1) {
            res += data_[v].query(y);
        }
        return res;
    }

    value_type query(const size_type x1, const size_type y1, const size_type x2, const size_type y2) const
    // returns sum for range {x1, y1}..{x2, y2}
    {
        if (x1 > x2 || y1 > y2) {
            return 0;
        }
        const value_type res_down_right = query(x2, y2);
        const value_type res_up_left = (x1 == 0 || y1 == 0 ? 0 : query(x1 - 1, y1 - 1));
        const value_type res_down_left = (y1 == 0 ? 0 : query(x2, y1 - 1));
        const value_type res_up_right = (x1 == 0 ? 0 : query(x1 - 1, y2));
        return res_down_right + res_up_left - res_down_left - res_up_right;
    }

private:
    container_type data_;
};
