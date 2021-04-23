#pragma once
#include <utility>
#include <vector>

template<typename T>
class Stack {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using rvalue = value_type&&;
    using container = std::vector<value_type>;

    explicit Stack(const size_type max_size) : data_(container(max_size)) {
        clear();
    }

    reference top() {
        return data_[top_index_ - 1];
    }

    const_reference top() const {
        return data_[top_index_ - 1];
    }

    constexpr bool empty() const {
        return top_index_ == 0;
    }

    void clear() {
        top_index_ = 0;
    }

    constexpr size_type size() const {
        return top_index_;
    }

    void push(const_reference value) {
        data_[top_index_++] = value;
    }

    void push(rvalue value) {
        data_[top_index_++] = std::move(value);
    }

    value_type pop() {
        return data_[--top_index_];
    }

    void swap(Stack& rhs) {
        data_.swap(rhs.data_);
        std::swap(top_index_, rhs.top_index_);
    }

    const container& data() const {
        return data_;
    }

    bool operator ==(const Stack& rhs) const {
        return top_index_ == rhs.top_index_ && data_ == rhs.data_;
    }

    bool operator !=(const Stack& rhs) const {
        return !operator==(rhs);
    }

    bool operator <(const Stack& rhs) const {
        return data_ < rhs.data_;
    }

    bool operator <=(const Stack& rhs) const {
        return data_ <= rhs.data_;
    }

    bool operator >(const Stack& rhs) const {
        return data_ > rhs.data_;
    }

    bool operator >=(const Stack& rhs) const {
        return data_ >= rhs.data_;
    }

private:
    container data_;
    size_type top_index_;
};
