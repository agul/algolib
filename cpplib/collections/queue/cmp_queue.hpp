#pragma once
#include <functional>

#include "collections/stack/cmp_stack.hpp"

template<typename T, typename Cmp = std::less<T>>
class CmpQueue {
public:
    using value_type = T;
    using size_type = std::size_t;
    using const_reference = const value_type&;

    explicit CmpQueue(const size_type n, const Cmp& cmp = Cmp()) : a_(n, cmp), b_(n, cmp), cmp_(cmp) {}

    constexpr bool empty() const {
        return a_.empty() && b_.empty();
    }

    value_type min_value() const {
        if (a_.empty()) {
            return b_.min_value();
        }
        if (b_.empty()) {
            return a_.min_value();
        }
        const value_type a = a_.min_value();
        const value_type b = b_.min_value();
        return cmp_(a, b) ? a : b;
    }

    void push(const_reference element) {
        a_.push(element);
    }

    value_type pop() {
        if (b_.empty()) {
            while (!a_.empty()) {
                b_.push(a_.pop());
            }
        }
        return b_.pop();
    }

    void clear() {
        a_.clear();
        b_.clear();
    }

private:
    CmpStack<value_type, Cmp> a_;
    CmpStack<value_type, Cmp> b_;
    const Cmp cmp_;
};
