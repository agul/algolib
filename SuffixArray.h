#pragma once
#include "Head.h"
#include "SegmentTree.h"
#include "SparseTable.h"

void suffixArrayCyclic(int sa[], const char * s, int n, const int alphabet = 27);
void suffixArrayCyclic(int sa[], const string& s, const int alphabet = 27);

void suffixLCPArrayCyclic(int sa[], int lcp[], const char * s, const int n, const int alphabet = 27);
void suffixLCPArrayCyclic(int sa[], int lcp[], const string& s, const int alphabet = 27);

class SuffixArray {
// class for internal functions for suffix array construction
public:

	static bool lessOrEqual(const int a1, const int a2, const int b1, const int b2);
	static bool lessOrEqual(const int a1, const int a2, const int a3, const int b1, const int b2, const int b3);
	static void radixPass(const int* a, int* b, const int * r, const int n, const int alphabet);
	static void build(const int * s, int* SA, const int n, const int alphabet);

};

void suffixArray(int sa[], const char * s, const int n, const int alphabet = 27);
void suffixArray(int sa[], const string& s, const int alphabet = 27);

void LCPArray(int lcp[], const int sa[], const char * s, const int n);
void LCPArray(int lcp[], const int sa[], const string& s);