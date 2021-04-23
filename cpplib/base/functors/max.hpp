#pragma once
#include <algorithm>

struct Max {
    template<typename T>
    T operator()(const T& lhs, const T& rhs) const {
        return std::max(lhs, rhs);
    }
};
