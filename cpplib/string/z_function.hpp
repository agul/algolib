#pragma once
#include <algorithm>
#include <vector>

#include "string_view.hpp"

template<typename size_type = std::size_t>
std::vector<size_type> z_function(const StringView& string) {
    std::vector<size_type> z(string.length(), 0);
    size_type l = 0;
    size_type r = 0;
    for (size_type i = 1; i < string.length(); ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < string.length() && string[z[i]] == string[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}
