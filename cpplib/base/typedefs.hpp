#pragma once
#include <utility>
#include <vector>

#ifdef _MSC_VER
#define LLD "%I64d"
#define LLU "%I64u"
#else
#define LLD "%lld"
#define LLU "%llu"
#endif

using ll = long long;
using ull = unsigned long long;
using uint = unsigned int;
using pii = std::pair<int, int>;
using pll = std::pair<ll, ll>;
using pli = std::pair<ll, int>;
using vi = std::vector<int>;
using vvi = std::vector<vi>;
using vp = std::vector<pii>;
using ld = long double;
