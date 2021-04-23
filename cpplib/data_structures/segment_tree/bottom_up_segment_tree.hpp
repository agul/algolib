#pragma once
#include <functional>
#include <vector>

#include "maths/bits.hpp"

template<typename T>
class BottomUpSegmentTree {
public:
    using functor_type = std::function<T(const T&, const T&)>;

    BottomUpSegmentTree(const size_t N, const functor_type& pred, const T& neutral_ = T(0)) :
            pred_(pred),
            N_(N),
            neutral_(neutral_) {
        offset_ = bit_ceil(N);
        size_ = offset_ << 1;
        data_.resize(size_);
        init();
    }

    constexpr size_t offset() const {
        return offset_;
    }

    constexpr size_t size() const {
        return size_;
    }

    constexpr size_t elements_count() const {
        return N_;
    }

    constexpr T neutral() const {
        return neutral_;
    }

    std::vector<T>& data() const {
        return data_;
    }

    void init() {
        std::fill_n(data_.begin(), data_.size(), neutral_);
    }

    void build(T a[]) {
        std::copy(a, a + N_, data_.begin() + offset_);
        for (size_t i = offset_ - 1; i >= 1; --i) {
            data_[i] = pred_(data_[i << 1], data_[(i << 1) ^ 1]);
        }
    }

    T query(const size_t left, const size_t right) const {
        T res = neutral_;
        for (size_t l = left + offset_, r = right + offset_; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) {
                res = pred_(data_[l++], res);
            }
            if ((r & 1) == 0) {
                res = pred_(data_[r--], res);
            }
        }
        return res;
    }

    void update(const size_t position, const T& value) {
        size_t pos = position + offset_;
        data_[pos] = value;
        for (pos >>= 1; pos >= 1; pos >>= 1) {
            data_[pos] = pred_(data_[pos << 1], data_[(pos << 1) ^ 1]);
        }
    }

private:
    std::vector<T> data_;
    const functor_type pred_;
    const T neutral_;
    size_t offset_;
    size_t size_;
    size_t N_;
};
