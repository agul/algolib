#pragma once
#include <functional>
#include <utility>

#include "stack.hpp"

template<typename T, typename Cmp = std::less<T>>
class CmpStack {
public:
    using value_type = T;
    using size_type = std::size_t;
    using const_reference = const value_type&;

    explicit CmpStack(const size_type n, const Cmp& cmp = Cmp()) : stack_(n), cmp_(cmp) {}

    constexpr bool empty() const {
        return stack_.empty();
    }

    value_type min_value() const {
        return stack_.empty() ? 0 : stack_.top().second;
    }

    void push(const_reference element) {
        value_type cur_min = element;
        if (!stack_.empty() && cmp_(stack_.top().second, cur_min)) {
            cur_min = stack_.top().second;
        }
        stack_.push(std::make_pair(element, cur_min));
    }

    value_type pop() {
        return stack_.pop().first;
    }

    void clear() {
        stack_.clear();
    }

private:
    Stack<std::pair<value_type, value_type>> stack_;
    const Cmp cmp_;
};
