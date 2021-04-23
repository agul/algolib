#pragma once
#include <functional>
#include <vector>

template<typename T, typename Cmp = std::less<T>>
class SparseTableCmp {
public:
    using value_type = T;
    using size_type = std::size_t;
    using comparator_type = Cmp;
    using container_type = std::vector<value_type>;

    explicit SparseTableCmp(const container_type& data, const comparator_type& cmp = comparator_type()) :
            size_(data.size()),
            cmp_(cmp),
            log_table_(size_ + 1, 0),
            data_(data)
    {
        for (size_type i = 2; i <= size_; ++i) {
            log_table_[i] = log_table_[i >> 1] + 1;
        }
        table_.assign(log_table_[size_] + 1, std::vector<size_type>(size_));

        std::iota(table_[0].begin(), table_[0].end(), 0);
        for (size_type level = 1, step = 2; step < size_; ++level, step <<= 1) {
            for (size_type i = 0; i + step <= size_; ++i) {
                const size_type x = table_[level - 1][i];
                const size_type y = table_[level - 1][i + (step >> 1)];
                table_[level][i] = (cmp_(data_.at(x), data_.at(y)) ? x : y);
            }
        }
    }

    size_type query(const size_type left, const size_type right) const
    // query for range [left, right)
    {
        const size_type level = log_table_[right - left - 1];
        const size_type x = table_[level][left];
        const size_type y = table_[level][right - (1 << level)];
        return (cmp_(data_.at(x), data_.at(y)) ? x : y);
    }

private:
    size_type size_;
    std::vector<std::vector<size_type>> table_;
    std::vector<size_type> log_table_;
    const comparator_type& cmp_;
    const container_type& data_;
};