#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <vector>

template<typename T>
class FenwickTreeSum {
public:
    using index_type = int32_t;
    static_assert(std::is_signed<index_type>::value, "Index type in fenwick tree should be signed integer");

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

    [[nodiscard]] size_type size() const {
        return data_.size();
    }

    [[nodiscard]] container_type& data() {
        return data_;
    }

    [[nodiscard]] const container_type& data() const {
        return data_;
    }

    void inc(const size_type index, const value_type delta) {
        for (size_type v = index; v < data_.size(); v = (v | (v + 1))) {
            data_[v] += delta;
        }
    }

    [[nodiscard]] value_type query(const size_type index) const
    // returns sum for range [0, index]
    {
        value_type res = 0;
        for (index_type v = index; v >= 0; v = (v & (v + 1)) - 1) {
            res += data_[v];
        }
        return res;
    }

    [[nodiscard]] value_type query(const size_type left, const size_type right) const
    // returns sum for range [left, right]
    {
        if (right < left) {
            return 0;
        }
        value_type res = 0;
        index_type r = right;
        index_type l = std::max(0, static_cast<index_type>(left) - 1);
        for (; r >= l; r = (r & (r + 1)) - 1) {
            res += data_[r];
        }
        if (left > 0) {
            for (; l > r; l = (l & (l + 1)) - 1) {
                res -= data_[l];
            }
        }
    }

private:
    container_type data_;
};
