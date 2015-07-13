#pragma once
#include "Head.h"

class DSU {
public:
	
	int * p, n, numSets;

	DSU(const int N) {
		p = new int[N];
	}

	~DSU() {
		delete[] p;
	}

	void init(const int _n);
	int findSet(const int v);
	bool unite(int a, int b);
	void finalize();

	int getNumSets() const {
		return numSets;
	}

private:
	DSU();

};