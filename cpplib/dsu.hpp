#pragma once
#include "Head.h"
#include "Maths.h"

class DSU {
public:
	
	int * p, n, numSets;

	DSU(const int N) {
		p = new int[N];
	}

	~DSU() {
		delete[] p;
	}

	void init(const int _n) {
		n = _n;
		srand(static_cast<uint32_t>(Random::GetRandSeed()));
		std::iota(p, p + n, 0);
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
			std::swap(a, b);
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

	size_t size() const {
		return n;
	}

	int getNumSets() const {
		return numSets;
	}

private:
	DSU();

};