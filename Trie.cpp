#include "Trie.h"

void Trie::init() 
// add initial state to trie
{
	sz = 1;
	states.emplace_back(Trie::State());
}

void Trie::clear() 
// delete all states
{
	/* for (int i = 0; i < sz; ++i) {
		states[i].next.clear();
		states[i].cnt = 0;
	} */
	states.clear();
}

Trie::State Trie::addChar(Trie::State& state, const char ch, const bool isTerminal)
// extend Trie by one char
// REQUIRE: char in interval [0..alphabet_size]
{
	if (state.next.count(ch)) {
		Trie::State& ret = states[state.next[ch]];
		ret.cnt += int(isTerminal);
		return ret;
	}
	state.next[ch] = sz++;
	states.emplace_back(Trie::State(int(isTerminal)));
	//states[sz].cnt = int(isTerminal);
	return states.back();
}

int Trie::addChar(const int v, const char ch, const bool isTerminal)
// extend Trie by one char
// REQUIRE: char in interval [0..alphabet_size]
{
	Trie::State& state = states[v];
	if (state.next.count(ch)) {
		int to = state.next[ch];
		states[to].cnt += int(isTerminal);
		return to;
	}
	int ret = sz;
	state.next[ch] = sz++;
	states.emplace_back(Trie::State(int(isTerminal)));
	//states[sz].cnt = int(isTerminal);
	return ret;
}

Trie::State Trie::addString(Trie::State& state, const char * s, const int n, const bool addSubstrings) 
// extend Trie by one string
// REQUIRE: all chars in interval [0..alphabet_size]
{
	Trie::State cur = state;
	for (int i = 0; i < n; ++i) {
		cur = addChar(cur, s[i], addSubstrings || i == n - 1);
	}
	return cur;
}

Trie::State Trie::addString(Trie::State& state, const std::string& s, const bool addSubstrings)
// extend Trie by one string
// REQUIRE: all chars in interval [0..alphabet_size]
{
	return addString(state, s.c_str(), s.length(), addSubstrings);
}

int Trie::addString(int v, const char * s, const int n, const bool addSubstrings)  
// extend Trie by one string
// REQUIRE: all chars in interval [0..alphabet_size]
{
	for (int i = 0; i < n; ++i) {
		v = addChar(v, s[i], addSubstrings || i == n - 1);
	}
	return v;
}

int Trie::addString(const int v, const std::string& s, const bool addSubstrings)
// extend Trie by one string
// REQUIRE: all chars in interval [0..alphabet_size]
{
	return addString(v, s.c_str(), s.length(), addSubstrings);
}
