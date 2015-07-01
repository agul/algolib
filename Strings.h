#pragma once
#include "Head.h"
#include "SegmentTree.h"

inline char upperCase(const char ch) {
	return (ch >= 'a' && ch <= 'z') ? ch ^ 32 : ch;
}

inline char lowerCase(const char ch) {
	return (ch >= 'A' && ch <= 'Z') ? ch ^ 32 : ch;
}

inline string upperCase(string s) {
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		s[i] = upperCase(s[i]);
	}
	return s;
}

inline string lowerCase(string s) {
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		s[i] = lowerCase(s[i]);
	}
	return s;
}

inline int dig(const char ch) {
	return ch - '0';
}

inline bool isAlpha(const char ch) {
	return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

inline bool isDigit(const char ch) {
	return (ch >= '0' && ch <= '9');
}

inline bool isLowerCase(const char ch) {
	return (ch >= 'a' && ch <= 'z');
}

inline bool isUpperCase(const char ch) {
	return (ch >= 'A' && ch <= 'Z');
}

template<class T> inline string toa(const T x)
// convert string to other types using sstream
{
	stringstream ss;
	ss << x;
	string ret;
	ss >> ret;
	return ret;
}

void prefixFunction(int p[], const char * s, int n = 0) {
	if (!n) {
		n = strlen(s);
	}
	p[0] = 0;
	for (int i = 1; i < n; ++i) {
		int j = p[i - 1];
		while (j && s[i] != s[j]) {
			j = p[j - 1];
		}
		if (s[i] == s[j]) {
			++j;
		}
		p[i] = j;
	}
}

void prefixFunction(int p[], const string& s) {
	prefixFunction(p, s.c_str(), s.length());
}

void zFunction(int z[], const char * s, int n = 0) {
	if (!n) {
		n = strlen(s);
	}
	z[0] = 0;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		z[i] = 0;
		if (i <= r) {
			z[i] = min(r - i + 1, z[i - l]);
		}
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			++z[i];
		}
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
}

void zFunction(int z[], const string& s) {
	zFunction(z, s.c_str(), s.length());
}

template<size_t N> struct SuffixAutomaton {

	struct State {
		int len, link, cnt;
		unordered_map<char, int> next;

		void setState(const int _len, const int _link, const unordered_map<char, int>& _next) {
			len = _len;
			link = _link;
			next = _next;
			cnt = 0;
		}

	};

	State states[N];
	int sz, last;

	void clear() {
		for (int i = 0; i < sz; ++i) {
			states[i].next.clear();
		}
	}

	void init() {
		sz = 1;
		last = 0;
		states[0].len = 0;
		states[0].link = -1;
	}

	void extend(const char ch) {
		int cur = sz++, p;
		states[cur].len = states[last].len + 1;
		for (p = last; p != -1 && !states[p].next.count(ch); p = states[p].link) {
			states[p].next[ch] = cur;
		}
		if (p == -1) {
			states[cur].link = 0;
		}
		else {
			int q = states[p].next[ch];
			if (states[p].len + 1 == states[q].len) {
				states[cur].link = q;
			}
			else {
				int clone = sz++;
				states[clone].setState(states[p].len + 1, states[q].link, states[q].next);
				for (; p != -1 && states[p].next[ch] == q; p = states[p].link) {
					states[p].next[ch] = clone;
				}
				states[q].link = states[cur].link = clone;
			}
		}
		last = cur;
		states[cur].cnt = 1;
	}

	void occurenceCount()
	// count occurence of substrings in text
	{
		int * val = new int[sz];
		for (int i = 0; i < sz; ++i) {
			val[i] = i;
		}
		sort(val, val + sz, [this](const int& a, const int& b) {
			return this->states[a].len > this->states[b].len;
		});
		for (int i = 0; i < sz; ++i) {
			int v = val[i], link = states[v].link;
			if (link != -1) {
				states[link].cnt += states[v].cnt;
			}
		}
		delete[] val;
	}

};

void suffixArrayCyclic(int sa[], const char * s, int n, const int alphabet = 27) 
// building suffix array for cyclic shifts, O(n log n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	int * cnt, *c[2], *pn;
	cnt = new int[max(255, n)];
	pn = new int[n];
	for (int i = 0; i < 2; ++i) {
		c[i] = new int[n];
	}
	memset(cnt, 0, alphabet * sizeof(int));
	for (int i = 0; i < n; ++i) {
		++cnt[s[i]];
	}
	for (int i = 1; i < alphabet; ++i) {
		cnt[i] += cnt[i - 1];
	}
	for (int i = 0; i < n; ++i) {
		sa[--cnt[s[i]]] = i;
	}
	c[0][sa[0]] = 0;
	int classes = 1;
	for (int i = 1; i < n; ++i) {
		if (s[sa[i]] != s[sa[i - 1]]) {
			++classes;
		}
		c[0][sa[i]] = classes - 1;
	}
	int h, ci, ni;
	for (h = 0; (1 << h) <= n; ++h) {
		cerr << h << " " << clock() << " " << n << endl;
		ci = h & 1, ni = ci ^ 1;
		for (int i = 0; i < n; ++i) {
			pn[i] = sa[i] - (1 << h);
			if (pn[i] < 0) {
				pn[i] += n;
			}
		}
		memset(cnt, 0, classes * sizeof(int));
		for (int i = 0; i < n; ++i) {
			++cnt[c[ci][pn[i]]];
		}
		for (int i = 1; i < classes; ++i) {
			cnt[i] += cnt[i - 1];
		}
		for (int i = n - 1; i >= 0; --i) {
			sa[--cnt[c[ci][pn[i]]]] = pn[i];
		}
		c[ni][sa[0]] = 0;
		classes = 1;
		for (int i = 1; i < n; ++i) {
			int mid1 = (sa[i] + (1 << h)) % n, mid2 = (sa[i - 1] + (1 << h)) % n;
			if (c[ci][sa[i]] != c[ci][sa[i - 1]] || c[ci][mid1] != c[ci][mid2]) {
				++classes;
			}
			c[ni][sa[i]] = classes - 1;
		}
	}
	delete[] cnt;
	delete[] pn;
	for (int i = 0; i < 2; ++i) {
		delete[] c[i];
	}
}

void suffixArrayCyclic(int sa[], const string& s, const int alphabet = 27)
// building suffix array for cyclic shifts, O(n log n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	suffixArrayCyclic(sa, s.c_str(), s.length(), alphabet);
}

void suffixLCPArrayCyclic(int sa[], int lcp[], const char * s, const int n, const int alphabet = 27)
// building suffix and LCP array for cyclic shifts, O(n log^2 n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	int *cnt, *c[2], *pn, *lpos, *rpos, *la[2];
	SegmentTreeMin<int> tree(n);
	cnt = new int[max(255, n)];
	pn = new int[n];
	lpos = new int[n];
	rpos = new int[n];
	memset(lpos, 0, sizeof(int)* n);
	memset(rpos, 0, sizeof(int)* n);
	for (int i = 0; i < 2; ++i) {
		c[i] = new int[n];
		la[i] = new int[n];
		memset(la[i], 0, sizeof(int)* n);
	}
	memset(cnt, 0, alphabet * sizeof(int));
	for (int i = 0; i < n; ++i) {
		++cnt[s[i]];
	}
	for (int i = 1; i < alphabet; ++i) {
		cnt[i] += cnt[i - 1];
	}
	for (int i = 0; i < n; ++i) {
		sa[--cnt[s[i]]] = i;
	}
	c[0][sa[0]] = 0;
	int classes = 1;
	for (int i = 1; i < n; ++i) {
		if (s[sa[i]] != s[sa[i - 1]]) {
			++classes;
		}
		c[0][sa[i]] = classes - 1;
	}
	int h, ci, ni;
	for (h = 0; (1 << h) <= n; ++h) {
		ci = h & 1, ni = ci ^ 1;
		for (int i = 0; i < n; ++i) {
			rpos[c[ci][sa[i]]] = i;
		}
		for (int i = n - 1; i >= 0; --i) {
			lpos[c[ci][sa[i]]] = i;
		}
		for (int i = 0; i < n; ++i) {
			pn[i] = sa[i] - (1 << h);
			if (pn[i] < 0) {
				pn[i] += n;
			}
		}
		memset(cnt, 0, classes * sizeof(int));
		for (int i = 0; i < n; ++i) {
			++cnt[c[ci][pn[i]]];
		}
		for (int i = 1; i < classes; ++i) {
			cnt[i] += cnt[i - 1];
		}
		for (int i = n - 1; i >= 0; --i) {
			sa[--cnt[c[ci][pn[i]]]] = pn[i];
		}
		c[ni][sa[0]] = 0;
		classes = 1;
		for (int i = 1; i < n; ++i) {
			int mid1 = (sa[i] + (1 << h)) % n, mid2 = (sa[i - 1] + (1 << h)) % n;
			if (c[ci][sa[i]] != c[ci][sa[i - 1]] || c[ci][mid1] != c[ci][mid2]) {
				++classes;
			}
			c[ni][sa[i]] = classes - 1;
		}
		tree.build(la[ci]);
		for (int i = 0; i < n - 1; ++i) {
			int a = sa[i], b = sa[i + 1];
			if (c[ci][a] != c[ci][b]) {
				la[ni][i] = la[ci][rpos[c[ci][a]]];
			}
			else {
				int aa = (a + (1 << h)) % n, bb = (b + (1 << h)) % n;
				la[ni][i] = (1 << h) + la[ci][tree.query(la[ci], lpos[c[ci][aa]], rpos[c[ci][bb]] - 1)];
				la[ni][i] = min(n, la[ni][i]);
			}
		}
	}
	for (int i = 0; i < n - 1; ++i) {
		lcp[i] = la[ni][i];
	}
	delete[] cnt;
	delete[] pn;
	delete[] lpos;
	delete[] rpos;
	for (int i = 0; i < 2; ++i) {
		delete[] c[i];
		delete[] la[i];
	}
}

void suffixLCPArrayCyclic(int sa[], int lcp[], const string& s, const int alphabet = 27)
// building suffix and LCP array for cyclic shifts, O(n log^2 n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	suffixLCPArrayCyclic(sa, lcp, s.c_str(), s.length(), alphabet);
}

struct SuffixArray {

	static inline bool lessOrEqual(const int a1, const int a2, const int b1, const int b2)
	//comparing pairs
	{
		return(a1 < b1 || (a1 == b1 && a2 <= b2));
	}

	static inline bool lessOrEqual(const int a1, const int a2, const int a3, const int b1, const int b2, const int b3)
	// comparing triples
	{
		return(a1 < b1 || (a1 == b1 && lessOrEqual(a2, a3, b2, b3)));
	}

	static void radixPass(const int* a, int* b, const int * r, const int n, const int alphabet) {
		int* cnt = new int[alphabet + 1];
		memset(cnt, 0, sizeof(int)* (alphabet + 1));
		for (int i = 0; i < n; i++) {
			++cnt[r[a[i]]];
		}
		for (int i = 0, sum = 0; i <= alphabet; i++) {
			int t = cnt[i];
			cnt[i] = sum;
			sum += t;
		}
		for (int i = 0; i < n; i++) {
			b[cnt[r[a[i]]]++] = a[i];
		}
		delete[] cnt;
	}

	static void build(const int * s, int* SA, const int n, const int alphabet)  {
		int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
		int* s12 = new int[n02 + 3];
		s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;
		int* SA12 = new int[n02 + 3];
		SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
		int* s0 = new int[n0];
		int* SA0 = new int[n0];
		for (int i = 0, j = 0; i < n + (n0 - n1); i++) {
			if (i % 3) {
				s12[j++] = i;
			}
		}
		radixPass(s12, SA12, s + 2, n02, alphabet);
		radixPass(SA12, s12, s + 1, n02, alphabet);
		radixPass(s12, SA12, s, n02, alphabet);
		int name = 0, c0 = -1, c1 = -1, c2 = -1;
		for (int i = 0; i < n02; i++) {
			if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2) {
				name++;
				c0 = s[SA12[i]];
				c1 = s[SA12[i] + 1];
				c2 = s[SA12[i] + 2];
			}
			s12[SA12[i] / 3 + (SA12[i] % 3 == 1 ? 0 : n0)] = name;
		}
		if (name < n02) {
			build(s12, SA12, n02, name);
			for (int i = 0; i < n02; i++) {
				s12[SA12[i]] = i + 1;
			}
		} else {
			for (int i = 0; i < n02; i++) {
				SA12[s12[i] - 1] = i;
			}
		}
		for (int i = 0, j = 0; i < n02; i++) {
			if (SA12[i] < n0) {
				s0[j++] = 3 * SA12[i];
			}
		}
		radixPass(s0, SA0, s, n0, alphabet);
		for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
			int i = GetI(), j = SA0[p];
			if (SA12[t] < n0 ?
				lessOrEqual(s[i], s12[SA12[t] + n0], s[j], s12[j / 3]) :
				lessOrEqual(s[i], s[i + 1], s12[SA12[t] - n0 + 1], s[j], s[j + 1], s12[j / 3 + n0])
				) 
			{
				SA[k] = i; 
				if (++t == n02) {
					for (k++; p < n0; p++, k++) {
						SA[k] = SA0[p];
					}
				}
			} else {
				SA[k] = j;
				if (++p == n0) {
					for (k++; t < n02; t++, k++) {
						SA[k] = GetI();
					}
				}
			}
		}
		delete[] s12;
		delete[] SA12;
		delete[] SA0;
		delete[] s0;
	}

};

void suffixArray(int sa[], const char * s, const int n, const int alphabet = 27) {
	if (n == 1) {
		sa[0] = 0;
		return;
	}
	int * str = new int[n + 3];
	for (int i = 0; i < n; ++i) {
		str[i] = s[i];
	}
	str[n] = str[n + 1] = str[n + 2] = 0;
	SuffixArray::build(str, sa, n, alphabet);
	delete[] str;
}

void suffixArray(int sa[], const string& s, const int alphabet = 27) {
	suffixArray(sa, s.c_str(), s.length(), alphabet);
}

void LCPArray(int lcp[], const int sa[], const char * s, const int n) {
	int k = 0;
	int * rank = new int[n];
	memset(rank, 0, sizeof(int)* n);
	for (int i = 0; i < n; i++) {
		rank[sa[i]] = i;
	}
	for (int i = 0; i<n; i++, k ? k-- : 0) {
		if (rank[i] == n - 1) {
			k = 0;
			continue;
		}
		int j = sa[rank[i] + 1];
		while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
			k++;
		}
		lcp[rank[i]] = k;
	}
	delete[] rank;
}

void LCPArray(int lcp[], const int sa[], const string& s) {
	LCPArray(lcp, sa, s.c_str(), s.length());
}

struct Trie {

	struct State {
		int cnt;
		unordered_map<char, int> next;

		State() : cnt(0) {}

		State(int cnt) : cnt(cnt) {}

		~State() {
			next.clear();
		}

	};

	int sz;
	vector<State> states;

	void init() {
		sz = 1;
		states.push_back(State());
	}

	void clear() {
		/* for (int i = 0; i < sz; ++i) {
			states[i].next.clear();
			states[i].cnt = 0;
		} */
		states.clear();
	}

	State addChar(State& state, const char ch, const bool isTerminal = false) {
		if (state.next.count(ch)) {
			State& ret = states[state.next[ch]];
			ret.cnt += int(isTerminal);
			return ret;
		}
		state.next[ch] = sz++;
		states.push_back(State(int(isTerminal)));
		//states[sz].cnt = int(isTerminal);
		return states.back();
	}

	int addChar(const int v, const char ch, const bool isTerminal = false) {
		State& state = states[v];
		if (state.next.count(ch)) {
			int to = state.next[ch];
			states[to].cnt += int(isTerminal);
			return to;
		}
		int ret = sz;
		state.next[ch] = sz++;
		states.push_back(State(int(isTerminal)));
		//states[sz].cnt = int(isTerminal);
		return ret;
	}

	State addString(State& state, const char * s, const int n, const bool addSubstrings = false) {
		State cur = state;
		for (int i = 0; i < n; ++i) {
			cur = addChar(cur, s[i], addSubstrings || i == n - 1);
		}
		return cur;
	}

	State addString(State& state, const string& s, const bool addSubstrings = false) {
		return addString(state, s.c_str(), s.length(), addSubstrings);
	}

	int addString(int v, const char * s, const int n, const bool addSubstrings = false) {
		for (int i = 0; i < n; ++i) {
			v = addChar(v, s[i], addSubstrings || i == n - 1);
		}
		return v;
	}

	int addString(const int v, const string& s, const bool addSubstrings = false) {
		return addString(v, s.c_str(), s.length(), addSubstrings);
	}

};