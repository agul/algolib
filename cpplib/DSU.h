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

	void init(const int _n);
	int findSet(const int v);
	bool unite(int a, int b);
	void finalize();

	size_t size() const {
		return n;
	}

	int getNumSets() const {
		return numSets;
	}

private:
	DSU();

};