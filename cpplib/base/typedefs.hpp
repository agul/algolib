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

#define ll long long
#define ull unsigned long long
#define uint unsigned int
#define pii std::pair<int, int>
#define pll std::pair<ll, ll>
#define pli std::pair<ll, int>
#define vi std::vector<int>
#define vvi std::vector<vi>
#define vp std::vector<pii>
#define ld long double
