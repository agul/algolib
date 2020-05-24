#pragma once
#include <vector>

#include "undirected_graph.hpp"

class LCA {
public:
	using size_type = std::size_t;

	template<typename T, size_type MASK>
	explicit LCA(const UndirectedGraph<T, MASK>& graph, const size_type starting_vertex = 0) :
			up_(graph.vertices_count()),
			tin_(graph.vertices_count()),
			tout_(graph.vertices_count()),
			timer_(0)
	{
		log_ = 1;
		while ((1 << log_) <= graph.vertices_count()) {
			++log_;
		}
		for (auto& it : up_) {
			it.resize(log_ + 1);
		}
		dfs(graph, starting_vertex, starting_vertex);
	}

	const std::vector<size_type>& tin() const {
		return tin_;
	}

	const std::vector<size_type>& tout() const {
		return tout_;
	}

	size_type tin(const size_type vertex) const {
		return tin_[vertex];
	}

	size_type tout(const size_type vertex) const {
		return tout_[vertex];
	}

	bool upper(const size_type lhs, const size_type rhs) const {
		const size_type time_in = tin_[rhs];
		return tin_[lhs] <= time_in && tout_[lhs] >= time_in;
	}

	size_type query(const size_type lhs, const size_type rhs) const {
		if (upper(lhs, rhs)) {
			return lhs;
		}
		if (upper(rhs, lhs)) {
			return rhs;
		}
		size_type vertex = lhs;
		for (const size_type i : inclusiveDownrange(log_)) {
			if (!upper(up_[vertex][i], rhs)) {
				vertex = up_[vertex][i];
			}
		}
		return up_[vertex][0];
	}

private:
	template<typename T, size_type MASK>
	void dfs(const UndirectedGraph<T, MASK>& graph, const size_type vertex, const size_type parent) {
		tin_[vertex] = timer_++;
		up_[vertex][0] = parent;
		for (size_type i = 1; i <= log_; ++i) {
			up_[vertex][i] = up_[up_[vertex][i - 1]][i - 1];
		}
		for (const auto& it : graph.edges(vertex)) {
			const size_type to = it.to();
			if (to == parent) {
				continue;
			}
			dfs(graph, to, vertex);
		}
		tout_[vertex] = timer_++;
	}

	std::vector<std::vector<size_type>> up_;
	std::vector<size_type> tin_;
	std::vector<size_type> tout_;
	size_type log_;
	size_type timer_;
};
