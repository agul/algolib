#pragma once
#include "Head.h"
#include "Maths.h"

class Hasher {
public:

	static const ll BASE = 277;
	static const int MOD = 1000000007;

	ll * hash, *deg;

	Hasher(int n) {
		hash = new ll[n + 1];
		deg = new ll[n + 1];
		calcPowers(deg, BASE, n);
	}

	~Hasher() {
		delete[] hash;
		delete[] deg;
	}

	void hashString(const char * s, int n = 0);
	void hashString(const string& s);

	ll getHash(const int l, const int r) const;

private:
	Hasher();

};

class DoubleHasher {
public:

	static const ll BASE[];
	static const int MOD[];

	ll * hash[2], * deg[2];

	DoubleHasher(int n) {
		for (int i = 0; i < 2; ++i) {
			hash[i] = new ll[n + 1];
			deg[i] = new ll[n + 1];
			calcPowers(deg[i], BASE[i], n, (ll)MOD[i]);
		}
	}

	~DoubleHasher() {
		for (int i = 0; i < 2; ++i) {
			delete[] hash[i];
			delete[] deg[i];
		}
	}

	void hashString(const char * s, int n = 0);
	void hashString(const string& s);

	ll getHash(const int l, const int r) const;

private:
	DoubleHasher();

};
