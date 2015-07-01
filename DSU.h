#pragma once
#include "Head.h"

struct DSU {
	
	int * p, n, numSets;

	DSU(const int N) {
		p = new int[N];
	}

	~DSU() {
		delete[] p;
	}

	void init(int _n) {
		n = _n;
		srand(time(0));
		for (int i = 0; i < n; ++i) {
			p[i] = i;
		}
		numSets = n;
	}

	int findSet(const int v) {
		if (v == p[v]) {
			return v;
		}
		return p[v] = findSet(p[v]);
	}

	bool unite(int a, int b) {
		a = findSet(a), b = findSet(b);
		if (a == b) {
			return false;
		}
		if (rand() & 1) {
			swap(a, b);
		}
		p[a] = b;
		--numSets;
		return true;
	}

	void finalize() {
		for (int i = 0; i < n; ++i) {
			p[i] = findSet(p[i]);
		}
	}

	int getNumSets() const {
		return numSets;
	}

};