#pragma once
#include "Head.h"

class SuffixAutomaton {
public:
	
	class State {
	public:
		int len, link, cnt;
		unordered_map<char, int> next;

		void setState(const int _len, const int _link, const unordered_map<char, int>& _next);

	};

	State * states;
	int sz, last;

	SuffixAutomaton(const int n) {
		states = new State[n << 1];
	}

	~SuffixAutomaton() {
		delete[] states;
	}

	void clear();
	void init();
	void extend(const char ch);
	void occurenceCount();

private:
	SuffixAutomaton();

};
