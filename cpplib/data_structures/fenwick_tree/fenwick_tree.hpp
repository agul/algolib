#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <vector>

template<typename T>
class FenwickTreeSum {
public:
    using value_type = T;
    using size_type = std::size_t;
    using container_type = std::vector<value_type>;

    explicit FenwickTreeSum(const size_type n) : data_(n, 0) {}

    explicit FenwickTreeSum(const std::vector<value_type>& a) : data_(a) {
        for (size_type i = 0; i < data_.size(); ++i) {
            const size_type tree_index = (i | (i + 1));
            if (tree_index < data_.size()) {
                data_[tree_index] += data_[i];
            }
        }
    }

    bool operator ==(const FenwickTreeSum& rhs) const {
        return data_ == rhs.data_;
    }

    void clear() {
        data_.assign(data_.size(), 0);
    }

    size_type size() const {
        return data_.size();
    }

    container_type& data() {
        return data_;
    }

    const container_type& data() const {
        return data_;
    }

    void inc(const size_type index, const value_type delta) {
        for (size_type v = index; v < data_.size(); v = (v | (v + 1))) {
            data_[v] += delta;
        }
    }

    value_type query(const size_type index) const
    // returns sum for range [0, index]
    {
        value_type res = 0;
        for (int32_t v = index; v >= 0; v = (v & (v + 1)) - 1) {
            res += data_[v];
        }
        return res;
    }

    value_type query(const size_type left, const size_type right) const
    // returns sum for range [left, right]
    {
        if (right < left) {
            return 0;
        }
        value_type res = 0;
        int32_t r = right;
        int32_t l = std::max(0, static_cast<int32_t>(left) - 1);
        for (; r >= l; r = (r & (r + 1)) - 1) {
            res += data_[r];
        }
        if (left > 0) {
            for (; l > r; l = (l & (l + 1)) - 1) {
                res -= data_[l];
            }
        }
        return res;
    }

private:
    container_type data_;
};
