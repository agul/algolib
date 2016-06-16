#pragma once
#include <algorithm>
#include <type_traits>
#include <vector>

#include "base/helpers.hpp"
#include "maths.hpp"
#include "range/ranges.hpp"
#include "queue.hpp"
	
enum GraphType {
	Weighted = 1
};

template<size_t MASK>
struct is_weighted 
{
	/// caide keep
	static constexpr bool value = (MASK & GraphType::Weighted) != 0;
};

template<typename T = long long, size_t MASK = 0>
class Graph {
public:
	using EdgesList = std::vector<size_t>;

	class Edge {
	public:
		explicit Edge(const std::vector<size_t>& from, const std::vector<size_t>& to,
			const std::vector<T>& weight, const size_t index) :
			from_(from), to_(to), weight_(weight), index_(index) {}

		size_t from() const {
			return from_[index_];
		}

		size_t to() const {
			return to_[index_];
		}

		template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
		T weight() const {
			return weight_[index_];
		}

		size_t id() const {
			return index_;
		}

		void set_index(const size_t index) {
			index_ = index;
		}

		Edge reversed() const {
			return Edge(to_, from_, weight_, index_);
		}

	private:
		const std::vector<size_t>& from_;
		const std::vector<size_t>& to_;
		const std::vector<T>& weight_;
		size_t index_;

	};

	template<typename Range>
	class EdgesHolder {
	public:

		template<typename BaseConstIterator>
		class EdgeConstIterator : public BaseConstIterator {
		public:
			using value_type = Edge;
			using pointer = const value_type*;
			using const_reference = const value_type&;

			explicit EdgeConstIterator(BaseConstIterator it, const std::vector<size_t>& from,
				const std::vector<size_t>& to, const std::vector<T>& weight) :
				BaseConstIterator(it), cur_edge_(from, to, weight, 0) {}

			pointer operator->() {
				return addressof(operator*());
			}

			const_reference operator*() {
				const typename BaseConstIterator::value_type index = this->BaseConstIterator::operator*();
				cur_edge_.set_index(index);
				return cur_edge_;
			}

		private:
			Edge cur_edge_;

		};

		using const_iterator = EdgeConstIterator<typename Range::const_iterator>;
		using value_type = typename const_iterator::value_type;

		explicit EdgesHolder(const Range& range, const std::vector<size_t>& from,
			const std::vector<size_t>& to, const std::vector<T>& weight) :
			begin_(range.begin(), from, to, weight), end_(range.end(), from, to, weight) {}

		const_iterator begin() const {
			return begin_;
		}

		const_iterator end() const {
			return end_;
		}

	private:
		const const_iterator begin_;
		const const_iterator end_;

	};

	Graph() = default;

	virtual ~Graph() {
		clear();
	}

	void init(const size_t vertex_count) {
		clear();
		vertex_count_ = vertex_count;
		edges_.resize(vertex_count_);
	}

	IntegerRange<size_t> vertices() const {
		return range(vertex_count_);
	}

	IntegerRange<size_t>::const_iterator begin() const {
		return vertices().begin();
	}

	IntegerRange<size_t>::const_iterator end() const {
		return vertices().end();
	}

	EdgesHolder<EdgesList> edges(const size_t vertex) const {
		return EdgesHolder<EdgesList>(edges_[vertex], from_, to_, weight_);
	}

	EdgesHolder<IntegerRange<size_t>> edges() const {
		return EdgesHolder<IntegerRange<size_t>>(range(from_.size()), from_, to_, weight_);
	}

	void clear();

	size_t from(const size_t index) const {
		return from_[index];
	}

	size_t to(const size_t index) const {
		return to_[index];
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	T weight(const size_t index) const {
		return weight_[index];
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	T weight_infinity() const {
		return std::numeric_limits<T>::max() / 2;
	}

	template<size_t Mask = MASK, typename std::enable_if<!is_weighted<Mask>::value>::type* = nullptr>
	void add_directed_edge(const size_t from, const size_t to) {
		push_edge(from, to);
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void add_directed_edge(const size_t from, const size_t to, const T weight) {
		weight_.emplace_back(weight);
		push_edge(from, to);
	}

	template<size_t Mask = MASK, typename std::enable_if<!is_weighted<Mask>::value>::type* = nullptr>
	void add_directed_edge(const Edge& edge) {
		add_directed_edge(edge.from(), edge.to());
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void add_directed_edge(const Edge& edge) {
		add_directed_edge(edge.from(), edge.to(), edge.weight());
	}

	bool is_sparse() const {
		return vertex_count_ == 0 || sqr(static_cast<ll>(vertex_count_)) >= (edges_count_ << 4);
	}

	size_t find_vertex_with_max_degree() const;

	bool is_bipartite(std::vector<size_t>& partition) const;

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	std::vector<std::vector<T>> floyd() const {
		auto dist = make_vector<T>(vertex_count_, vertex_count_, weight_infinity());
		for (const auto v : vertices()) {
			dist[v][v] = 0;
		}
		for (const auto& edge : edges()) {
			umin(dist[edge.from()][edge.to()], edge.weight());
		}
		for (const auto k : vertices()) {
			for (const auto i : vertices()) {
				for (const auto j : vertices()) {
					umin(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}
		return dist;
	}

protected:
	void push_edge(const size_t from, const size_t to) {
		const size_t edge_id = from_.size();
		from_.emplace_back(from);
		to_.emplace_back(to);
		edges_[from].emplace_back(edge_id);
	}

	std::vector<EdgesList> edges_;
	std::vector<size_t> from_;
	std::vector<size_t> to_;
	std::vector<T> weight_;

	size_t vertex_count_;

};

template<typename T, size_t MASK>
void Graph<T, MASK>::clear() {
	vertex_count_ = 0;
	edges_.clear();
	from_.clear();
	to_.clear();
	weight_.clear();
}

template<typename T, size_t MASK>
size_t Graph<T, MASK>::find_vertex_with_max_degree() const {
	const auto iter = std::max_element(edges_.begin(), edges_.end(),
		[](const EdgesList& lhs, const EdgesList& rhs) {
		return lhs.size() < rhs.size();
	});
	return static_cast<size_t>(std::distance(edges_.begin(), iter));
}

template<typename T, size_t MASK>
bool Graph<T, MASK>::is_bipartite(std::vector<size_t>& partition) const {
	const size_t kNone = std::numeric_limits<size_t>::max();
	partition.assign(vertex_count_, kNone);
	Queue<size_t> q(vertex_count_);
	for (size_t i = 0; i < vertex_count_; ++i) {
		if (partition[i] == kNone) {
			q.clear();
			q.push(i);
			partition[i] = 0;
			while (!q.empty()) {
				const size_t vertex = q.pop_front();
				const size_t color = partition[vertex];
				for (const auto& it : edges(vertex)) {
					const size_t to_vertex = it.to();
					if (partition[to_vertex] == kNone) {
						partition[to_vertex] = color ^ 1;
						q.push(to_vertex);
						continue;
					}
					if (partition[to_vertex] == color) {
						return false;
					}
				}
			}
		}
	}
	return true;
}
