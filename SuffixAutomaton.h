#pragma once
#include "Head.h"

template<size_t N>
class SuffixAutomaton {
public:
	
	class State {
	public:
		int len, link, cnt;
		unordered_map<char, int> next;

		void setState(const int _len, const int _link, const unordered_map<char, int>& _next);

	};

	State states[1000];
	int sz, last;

	void clear();
	void init();
	void extend(const char ch);
	void occurenceCount();

};

template<size_t N>
void SuffixAutomaton<N>::State::setState(const int _len, const int _link, const unordered_map<char, int>& _next)
// assign values to the state, similar to constructor
{
	len = _len;
	link = _link;
	next = _next;
	cnt = 0;
}

template<size_t N>
void SuffixAutomaton<N>::clear()
// clear all automaton states
{
	for (int i = 0; i < sz; ++i) {
		states[i].next.clear();
	}
}

template<size_t N>
void SuffixAutomaton<N>::init()
// adding initial state to automaton
{
	sz = 1;
	last = 0;
	states[0].len = 0;
	states[0].link = -1;
}

template<size_t N>
void SuffixAutomaton<N>::extend(const char ch) {
	int cur = sz++, p;
	states[cur].len = states[last].len + 1;
	for (p = last; p != -1 && !states[p].next.count(ch); p = states[p].link) {
		states[p].next[ch] = cur;
	}
	if (p == -1) {
		states[cur].link = 0;
	}
	else {
		int q = states[p].next[ch];
		if (states[p].len + 1 == states[q].len) {
			states[cur].link = q;
		}
		else {
			int clone = sz++;
			states[clone].setState(states[p].len + 1, states[q].link, states[q].next);
			for (; p != -1 && states[p].next[ch] == q; p = states[p].link) {
				states[p].next[ch] = clone;
			}
			states[q].link = states[cur].link = clone;
		}
	}
	last = cur;
	states[cur].cnt = 1;
}

template<size_t N>
void SuffixAutomaton<N>::occurenceCount()
// count occurence of substrings in text
{
	int * val = new int[sz];
	for (int i = 0; i < sz; ++i) {
		val[i] = i;
	}
	sort(val, val + sz, [this](const int& a, const int& b) {
		return this->states[a].len > this->states[b].len;
	});
	for (int i = 0; i < sz; ++i) {
		int v = val[i], link = states[v].link;
		if (link != -1) {
			states[link].cnt += states[v].cnt;
		}
	}
	delete[] val;
}
