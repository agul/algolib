#include "SuffixArray.h"

void suffixArrayCyclic(int sa[], const char * s, int n, const int alphabet) 
// building suffix array for cyclic shifts, O(n log n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	int * cnt, *c[2], *pn;
	cnt = new int[max(255, n)];
	pn = new int[n];
	for (int i = 0; i < 2; ++i) {
		c[i] = new int[n];
	}
	fill_n(cnt, alphabet, 0);
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
		fill_n(cnt, classes, 0);
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

void suffixArrayCyclic(int sa[], const string& s, const int alphabet)
// building suffix array for cyclic shifts, O(n log n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	suffixArrayCyclic(sa, s.c_str(), s.length(), alphabet);
}

void suffixLCPArrayCyclic(int sa[], int lcp[], const char * s, const int n, const int alphabet)
// building suffix and LCP array for cyclic shifts, O(n log^2 n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	int *cnt, *c[2], *pn, *lpos, *rpos, *la[2];
	SegmentTreeMin<int> tree(n);
	cnt = new int[max(255, n)];
	pn = new int[n];
	lpos = new int[n];
	rpos = new int[n];
	fill_n(lpos, n, 0);
	fill_n(rpos, n, 0);
	for (int i = 0; i < 2; ++i) {
		c[i] = new int[n];
		la[i] = new int[n];
		fill_n(la[i], n, 0);
	}
	fill_n(cnt, alphabet, 0);
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
		fill_n(cnt, classes, 0);
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

void suffixLCPArrayCyclic(int sa[], int lcp[], const string& s, const int alphabet)
// building suffix and LCP array for cyclic shifts, O(n log^2 n)
// to prevent using cyclic shifts consider n = n + 1; s[n] = 0;
{
	suffixLCPArrayCyclic(sa, lcp, s.c_str(), s.length(), alphabet);
}

bool SuffixArray::lessOrEqual(const int a1, const int a2, const int b1, const int b2)
//comparing pairs
{
	return(a1 < b1 || (a1 == b1 && a2 <= b2));
}
bool SuffixArray::lessOrEqual(const int a1, const int a2, const int a3, const int b1, const int b2, const int b3)
// comparing triples
{
	return(a1 < b1 || (a1 == b1 && lessOrEqual(a2, a3, b2, b3)));
}

void SuffixArray::radixPass(const int* a, int* b, const int * r, const int n, const int alphabet) {
	int* cnt = new int[alphabet + 1];
	fill_n(cnt, alphabet + 1, 0);
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

void SuffixArray::build(const int * s, int* SA, const int n, const int alphabet)  {
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

void suffixArray(int sa[], const char * s, const int n, const int alphabet)
// constructing suffix array in O(N)
// REQUIRE: all chars in interval [0..alphabet)
{
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

void suffixArray(int sa[], const string& s, const int alphabet)
// constructing suffix array in O(N)
// REQUIRE: all chars in interval [0..alphabet)
{
	suffixArray(sa, s.c_str(), s.length(), alphabet);
}

void LCPArray(int lcp[], const int sa[], const char * s, const int n)
// constructing LCP array using suffix array in O(N)
{
	int k = 0;
	int * rank = new int[n];
	fill_n(rank, n, 0);
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

void LCPArray(int lcp[], const int sa[], const string& s)
// constructing LCP array using suffix array in O(N)
{
	LCPArray(lcp, sa, s.c_str(), s.length());
}
