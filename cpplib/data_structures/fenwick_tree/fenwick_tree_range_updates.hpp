#pragma once
#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <vector>

template<typename T>
class FenwickTreeSumRangeUpdates {
public:
    using index_type = int32_t;
    static_assert(std::is_signed<index_type>::value, "Index type in fenwick tree should be signed integer");

    using size_type = std::size_t;
    using value_type = T;
    using container_type = std::vector<value_type>;

    explicit FenwickTreeSumRangeUpdates(const size_type n) : add_(n, 0), mul_(n, 0) {}

    explicit FenwickTreeSumRangeUpdates(const std::vector<value_type>& a) : FenwickTreeSumRangeUpdates(a.size()) {
        for (size_type i = 0; i < a.size(); ++i) {
            update(i, i, a[i]);
        }
    }

    void clear() {
        add_.assign(add_.size(), 0);
        mul_.assign(mul_.size(), 0);
    }

    void update(const index_type left, const index_type right, const value_type delta) {
        if (right < left) {
            return;
        }
        update_impl(left, delta, -delta * (left - 1));
        update_impl(right, -delta, delta * right);
    }

    [[nodiscard]] value_type query(index_type at) const {
        value_type result_multiply = 0;
        value_type result_add = 0;
        const index_type start = at;
        while (at >= 0) {
            result_multiply += mul_[at];
            result_add += add_[at];
            at = (at & (at + 1)) - 1;
        }
        return result_multiply * start + result_add;
    }

    [[nodiscard]] value_type query(const index_type left, const index_type right) const {
        return query(right) - query(left - 1);
    }

private:
    container_type add_;
    container_type mul_;

    void update_impl(index_type at, const value_type update_multiply, const value_type update_add) {
        while (at < add_.size()) {
            mul_[at] += update_multiply;
            add_[at] += update_add;
            at |= (at + 1);
        }
    }
};