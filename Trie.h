#pragma once
#include "Head.h"

class Trie {
public:

	class State {
	public:
		int cnt;
		unordered_map<char, int> next;

		State() : cnt(0) {}
		State(const int cnt) : cnt(cnt) {}
		~State() {
			next.clear();
		}

	};

	int sz;
	vector<State> states;

	void init();
	void clear();

	State addChar(State& state, const char ch, const bool isTerminal = false);
	int addChar(const int v, const char ch, const bool isTerminal = false);

	State addString(State& state, const char * s, const int n, const bool addSubstrings = false);
	State addString(State& state, const string& s, const bool addSubstrings = false);
	int addString(int v, const char * s, const int n, const bool addSubstrings = false);
	int addString(const int v, const string& s, const bool addSubstrings = false);

};