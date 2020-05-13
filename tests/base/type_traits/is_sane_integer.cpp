#include "base/type_traits/is_sane_integer.hpp"
#include <cstdint>
#include <vector>


static_assert(is_sane_integer<int>::value);
static_assert(is_sane_integer<const int>::value);
static_assert(is_sane_integer<int64_t>::value);

static_assert(!is_sane_integer<double>::value);
static_assert(!is_sane_integer<char>::value);
static_assert(!is_sane_integer<const char>::value);
static_assert(!is_sane_integer<std::vector<int>>::value);
