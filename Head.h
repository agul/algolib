#pragma once
#pragma comment(linker, "/STACK:66777216")
#define _USE_MATH_DEFINES
#include <cstdio>
#pragma warning(disable : 4996)
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <utility>
#include <functional>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cassert>
#include <queue>
#include <cmath>
#include <random>
#include <sstream>
#include <numeric>
#include <limits>
#include <chrono>
#pragma hdrstop

using namespace std;

#ifdef _WIN32
#define LLD "%I64d"
#define LLU "%I64u"
#define popcount(a) __popcnt(a)
#else
#define LLD "%lld"
#define LLU "%llu"
#define popcount(a) __builtin_popcount(a)
#endif

#define fill(a, x) memset(a, x, sizeof(a))
#define sz(a) ((int)a.size())
#define X first
#define Y second
#define mp make_pair
#define all(a) a.begin(), a.end()
#define pb push_back

#define ll long long
#define ull unsigned long long
#define uint unsigned int
#define pii pair<int, int>
#define pll pair<ll, ll>
#define vi vector<int>
#define vvi vector<vi>
#define vp vector<pii>
#define ld long double

const int INF = 0x3f3f3f3f;
const double EPS = 1e-9;
const ll LINF = 0x3f3f3f3f3f3f3f3fLL;

const int DX[] = { -1, 0, 1, 0, -1, -1, 1, 1};
const int DY[] = { 0, -1, 0, 1, -1, 1, 1, -1};

void inputFiles();

template<class T> void addMod(T& a, const T& b, const T& mod = 1000000007) {
	if ((a += b) >= mod) {
		a -= mod;
	}
}

template<class T> void umin(T& a, const T& b) {
	if (b < a) {
		a = b;
	}
}

template<class T> void umax(T& a, const T& b) {
	if (a < b) {
		a = b;
	}
}
