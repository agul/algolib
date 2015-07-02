#include "DSU.h"

void DSU::init(const int _n) {
	n = _n;
	srand(time(0));
	for (int i = 0; i < n; ++i) {
		p[i] = i;
	}
	numSets = n;
}

int DSU::findSet(const int v) {
	if (v == p[v]) {
		return v;
	}
	return p[v] = findSet(p[v]);
}

bool DSU::unite(int a, int b) {
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

void DSU::finalize() {
	for (int i = 0; i < n; ++i) {
		p[i] = findSet(p[i]);
	}
}

