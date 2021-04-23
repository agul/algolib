#pragma once
#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

#include "maths/bits.hpp"

template<size_t N>
class Bitset {
public:
    using value_type = uint64_t;
    using size_type = std::size_t;
    using container_type = std::vector<value_type>;

    static constexpr size_type kValueBitWidth = std::numeric_limits<value_type>::digits;
    static constexpr size_type kIndexMask = kValueBitWidth - 1;
    static constexpr size_type kIndexPower = binary_power(kValueBitWidth);

    static constexpr size_type kBitsetLength = (N >> kIndexPower) + 1;

    Bitset() : data_(kBitsetLength, 0) {}

    Bitset& set(const size_type index) {
        data_[index >> kIndexPower] |= (1LL << (index & kIndexMask));
        return *this;
    }

    Bitset& flip(const size_type index) {
        data_[index >> kIndexPower] ^= (1LL << (index & kIndexMask));
        return *this;
    }

    Bitset& clear(const size_type index) {
        data_[index >> kIndexPower] &= ~(1LL << (index & kIndexMask));
        return *this;
    }

    bool get(const size_type index) const {
        return data_[index >> kIndexPower] & (1LL << (index & kIndexMask));
    }

    bool operator[](const size_type index) const {
        return get(index);
    }

    container_type& data() {
        return data_;
    }

    const container_type& data() const {
        return data_;
    }

    size_type size() const {
        return N;
    }

    bool any() const {
        return std::any_of(data_.cbegin(), data_.cend(), [](const value_type x) {
            return x != 0;
        });
    }

    bool none() const {
        return !any();
    }

    void swap(Bitset& rhs) {
        data_.swap(rhs.data_);
    }

    Bitset& reset() {
        data_.assign(kBitsetLength, 0);
        return *this;
    }

    Bitset& set() {
        data_.assign(kBitsetLength, std::numeric_limits<value_type>::max());
        return *this;
    }

    Bitset& flip() {
        std::transform(data_.cbegin(), data_.cend(), data_.begin(), [](const value_type value) {
            return ~value;
        });
        return *this;
    }

    Bitset operator ~() const {
        Bitset ret = *this;
        ret.flip();
        return ret;
    }

    size_type count() const {
        return std::accumulate(data_.cbegin(), data_.cend(), 0, [](const size_type acc, value_type value) {
            return acc + popcount(value);
        });
    }

    bool equal(const Bitset& rhs) const {
        return data_ == rhs.data_;
    }

    bool operator ==(const Bitset& rhs) const {
        return equal(rhs);
    }

    bool operator !=(const Bitset& rhs) const {
        return !equal(rhs);
    }

    Bitset bit_and(const Bitset& rhs) const {
        Bitset ret;
        for (size_type i = 0; i < kBitsetLength; ++i) {
            ret.data_[i] = data_[i] & rhs.data_[i];
        }
        return ret;
    }

    Bitset operator &(const Bitset& rhs) const {
        return bit_and(rhs);
    }

    Bitset& operator &=(const Bitset& rhs) {
        Bitset res = bit_and(rhs);
        swap(res);
        return *this;
    }

    Bitset bit_or(const Bitset& rhs) const {
        Bitset ret;
        for (size_type i = 0; i < kBitsetLength; ++i) {
            ret.data_[i] = data_[i] | rhs.data_[i];
        }
        return ret;
    }

    Bitset operator |(const Bitset& rhs) const {
        return bit_or(rhs);
    }

    Bitset& operator |=(const Bitset& rhs) {
        Bitset res = bit_or(rhs);
        swap(res);
        return *this;
    }

    Bitset bit_xor(const Bitset& rhs) const {
        Bitset ret;
        for (size_type i = 0; i < kBitsetLength; ++i) {
            ret.data_[i] = data_[i] ^ rhs.data_[i];
        }
        return ret;
    }

    Bitset operator ^(const Bitset& rhs) const {
        return bit_xor(rhs);
    }

    Bitset& operator ^=(const Bitset& rhs) {
        Bitset res = bit_xor(rhs);
        swap(res);
        return *this;
    }

    bool operator <(const Bitset& rhs) const {
        return data_ < rhs.data_;
    }

    Bitset rotate_left(const size_t shift) const {
        Bitset ret;
        const size_type full_shift = shift >> kIndexPower;
        const size_type rem_shift = shift & kIndexMask;
        for (size_type i = 0; i < kBitsetLength; ++i) {
            size_type index = i + full_shift;
            if (index >= kBitsetLength) {
                index -= kBitsetLength;
            }
            size_type prev = i - 1;
            if (i == 0) {
                prev = kBitsetLength - 1;
            }
            ret.data_[index] = (data_[i] << rem_shift) | (data_[prev] >> (kValueBitWidth - rem_shift));
        }
        return ret;
    }

    size_type first_bit_from(const size_type index) const {
        if (index >= size()) {
            return index;
        }
        const value_type mask_from_index = ~((static_cast<value_type>(1) << (index & kIndexMask)) - 1);
        const value_type first_block = data_[index >> kIndexPower];
        const value_type value = mask_from_index & first_block;
        if (value != 0) {
            return (index & ~static_cast<value_type>(kIndexMask)) + countr_zero(value);
        }
        const auto it = std::find_if(data_.cbegin() + (index >> kIndexPower) + 1, data_.cend(), [](const value_type x) {
            return x != 0;
        });
        if (it == data_.cend()) {
            return size();
        }
        return (it - data_.cbegin()) * kValueBitWidth + countr_zero(*it);
    }

    size_type first_bit_after(const size_type index) const {
        return first_bit_from(index + 1);
    }

    size_type least_significant_bit() const {
        return first_bit_from(0);
    }

    std::vector<size_type> indices() const {
        std::vector<size_type> res;
        size_type index = least_significant_bit();
        while (index < size()) {
            res.emplace_back(index);
            index = first_bit_after(index);
        }
        return res;
    }

private:
    container_type data_;
};
