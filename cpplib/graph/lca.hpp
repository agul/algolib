#pragma once
#include <vector>

#include "../graph/undirected_graph.hpp"

class LCA {
public:
	template<typename T, size_t MASK>
	LCA(const UndirectedGraph<T, MASK>& graph) : up_(graph.vertices_count()), tin_(graph.vertices_count()),
		tout_(graph.vertices_count()), timer_(0) {
		log_ = 1;
		while ((1 << log_) <= graph.vertices_count()) {
			++log_;
		}
		for (auto& it : up_) {
			it.resize(log_ + 1);
		}
		dfs(graph, 0, 0);
	}

	const std::vector<size_t> tin() const {
		return tin_;
	}

	const std::vector<size_t> tout() const {
		return tout_;
	}

	bool upper(const size_t lhs, const size_t rhs) const {
		const size_t time_in = tin_[rhs];
		return tin_[lhs] <= time_in && tout_[lhs] >= time_in;
	}

	size_t query(const size_t lhs, const size_t rhs) const {
		if (upper(lhs, rhs)) {
			return lhs;
		}
		if (upper(rhs, lhs)) {
			return rhs;
		}
		size_t vertex = lhs;
		for (const size_t i : inclusiveDownrange(log_)) {
			if (!upper(up_[vertex][i], rhs)) {
				vertex = up_[vertex][i];
			}
		}
		return up_[vertex][0];
	}

private:
	template<typename T, size_t MASK>
	void dfs(const UndirectedGraph<T, MASK>& graph, const size_t vertex, const size_t parent) {
		tin_[vertex] = timer_++;
		up_[vertex][0] = parent;
		for (size_t i = 1; i <= log_; ++i) {
			up_[vertex][i] = up_[up_[vertex][i - 1]][i - 1];
		}
		for (const auto& it : graph.edges(vertex)) {
			const size_t to = it.to();
			if (to == parent) {
				continue;
			}
			dfs(graph, to, vertex);
		}
		tout_[vertex] = timer_++;
	}

	std::vector<std::vector<size_t>> up_;
	std::vector<size_t> tin_;
	std::vector<size_t> tout_;
	size_t log_;
	size_t timer_;
};
