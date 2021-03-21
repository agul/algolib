#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "string/string_view.hpp"

class Trie {
public:

	class State {
	public:
		using container = std::unordered_map<char, size_t>;

		State() : State(0) {}

		explicit State(const size_t cnt) : cnt_(cnt) {}

		const container& next() const {
			return next_;
		}

		size_t next(const char to) const {
			return next_.at(to);
		}

		bool has_edge(const char to) const {
			return next_.find(to) != next_.cend();
		}

		size_t count() const {
			return cnt_;
		}

		void increment_count() {
			++cnt_;
		}

		void add_edge(const char to, const size_t state_id) {
			next_[to] = state_id;
		}

	private:
		container next_;
		size_t cnt_;
	};

	Trie() {
		init();
	}

	void init() {
		states_.resize(1);
	}

	void clear() {
		states_.clear();
	}

	State& add_char(State& state, const char ch, const bool is_terminal = false)
		// extend Trie by one char
		// REQUIRE: char in interval [0..alphabet_size]
	{
		if (state.has_edge(ch)) {
			State& ret = states_[state.next(ch)];
			if (is_terminal) {
				ret.increment_count();
			}
			return ret;
		}
		state.add_edge(ch, states_.size());
		states_.emplace_back(is_terminal);
		return states_.back();
	}

	size_t add_char(const size_t vertex, const char ch, const bool is_terminal = false)
		// extend Trie by one char
		// REQUIRE: char in interval [0..alphabet_size]
	{
		State& state = states_[vertex];
		if (state.has_edge(ch)) {
			const size_t to = state.next(ch);
			if (is_terminal) {
				states_[to].increment_count();
			}
			return to;
		}
		state.add_edge(ch, states_.size());
		states_.emplace_back(is_terminal);
		return states_.size() - 1;
	}

	State& add_string(State& state, const StringView& s, const bool add_substrings = false)
		// extend Trie by one string
		// REQUIRE: all chars in interval [0..alphabet_size]
	{
		State* cur = &state;
		for (size_t i = 0; i < s.length(); ++i) {
			cur = &add_char(*cur, s[i], add_substrings || i == s.length() - 1);
		}
		return *cur;
	}

	size_t add_string(const size_t vertex, const StringView& s, const bool add_substrings = false)
		// extend Trie by one string
		// REQUIRE: all chars in interval [0..alphabet_size]
	{
		size_t v = vertex;
		for (size_t i = 0; i < s.length(); ++i) {
			v = add_char(v, s[i], add_substrings || i == s.length() - 1);
		}
		return v;
	}

	const std::vector<State>& states() const {
		return states_;
	}

	const State& states(const size_t index) const {
		return states_[index];
	}

private:
	std::vector<State> states_;
};
