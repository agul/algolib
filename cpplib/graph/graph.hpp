#pragma once
#include <algorithm>
#include <type_traits>
#include <vector>

#include "../base/helpers.hpp"
#include "../maths/maths.hpp"
#include "../range/ranges.hpp"
#include "../queue.hpp"
	
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
		constexpr explicit Edge(const std::vector<size_t>& from, const std::vector<size_t>& to,
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

		constexpr size_t id() const {
			return index_;
		}

		void set_index(const size_t index) {
			index_ = index;
		}

		constexpr Edge reversed() const {
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

			constexpr explicit EdgeConstIterator(BaseConstIterator it, const std::vector<size_t>& from,
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

		constexpr explicit EdgesHolder(const Range& range, const std::vector<size_t>& from,
			const std::vector<size_t>& to, const std::vector<T>& weight) :
			begin_(range.begin(), from, to, weight), end_(range.end(), from, to, weight) {}

		constexpr const_iterator begin() const {
			return begin_;
		}

		constexpr const_iterator end() const {
			return end_;
		}

		constexpr size_t size() const {
			return end_ - begin_;
		}

	private:
		const const_iterator begin_;
		const const_iterator end_;

	};

	Graph() : Graph(0) {}
	Graph(const size_t vertices_count) : vertices_count_(vertices_count), edges_(vertices_count) {}

	virtual ~Graph() {
		clear();
	}

	void init(const size_t vertices_count) {
		clear();
		vertices_count_ = vertices_count;
		edges_.resize(vertices_count_);
	}

	IntegerRange<size_t> vertices() const {
		return range(vertices_count_);
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

	size_t vertices_count() const {
		return vertices_count_;
	}

	size_t edges_count() const {
		return from_.size();
	}

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
		return vertices_count_ == 0 || sqr(static_cast<long long>(vertices_count_)) >= (edges_count() << 4);
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	std::vector<std::vector<T>> floyd() const {
		auto dist = make_vector<T>(vertices_count_, vertices_count_, weight_infinity());
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
	
	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void dijkstra(const size_t start_vertex, std::vector<T>* distance, std::vector<size_t>* last_edge = nullptr) const {
		if (is_sparse() && false) {
			sparse_dijkstra(start_vertex, distance, last_edge);
		} else {
			dense_dijkstra(start_vertex, distance, last_edge);
		}
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	T dijkstra(const size_t start_vertex, const size_t finish_vertex, std::vector<size_t>* path = nullptr) const {
		if (start_vertex == finish_vertex) {
			return 0;
		}
		std::vector<T> dist;
		std::vector<size_t> last;
		dijkstra(start_vertex, &dist, &last);
		if (path != nullptr) {
			std::vector<size_t> tmp_path;
			size_t vertex = finish_vertex;
			while (vertex != start_vertex) {
				const size_t edge = last[vertex];
				tmp_path.emplace_back(edge);
				vertex = from_[edge];
			}
			std::reverse(tmp_path.begin(), tmp_path.end());
			path->swap(tmp_path);
		}
		return dist[finish_vertex];
	}

	size_t find_vertex_with_max_degree() const;

	bool is_bipartite(std::vector<size_t>& partition) const;
	void maximal_matching(std::vector<size_t>* match) const;

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

	size_t vertices_count_;

private:
	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void sparse_dijkstra(const size_t start_vertex, std::vector<T>* distance, std::vector<size_t>* last_edge) const {
		std::vector<T> dist(vertices_count_, weight_infinity());
		std::vector<size_t> last(vertices_count_, std::numeric_limits<size_t>::max());
		dist[start_vertex] = 0;
		using QueueNode = std::pair<T, size_t>;
		std::priority_queue<QueueNode> q;
		q.emplace(0, start_vertex);
		while (!q.empty()) {
			const QueueNode node = q.top();
			q.pop();
			const T len = -node.first;
			const size_t vertex = node.second;
			if (len > dist[vertex]) {
				continue;
			}
			for (const auto& it : edges(vertex)) {
				const T new_dist = len + it.weight();
				const size_t to = it.to();
				if (umin(dist[to], new_dist)) {
					q.push(std::make_pair(-new_dist, to));
					last[to] = it.id();
				}
			}
		}
		distance->swap(dist);
		if (last_edge != nullptr) {
			last_edge->swap(last);
		}
	}

	template<size_t Mask = MASK, typename std::enable_if<is_weighted<Mask>::value>::type* = nullptr>
	void dense_dijkstra(const size_t start_vertex, std::vector<T>* distance, std::vector<size_t>* last_edge) const {
		std::vector<bool> used(vertices_count_);
		std::vector<T> dist(vertices_count_, weight_infinity());
		std::vector<size_t> last(vertices_count_, std::numeric_limits<size_t>::max());
		dist[start_vertex] = 0;
		for (size_t iter = 0; iter < vertices_count_; ++iter) {
			size_t vertex = std::numeric_limits<size_t>::max();
			T min_dist = weight_infinity();
			for (const size_t v : vertices()) {
				if (!used[v] && umin(min_dist, dist[v])) {
					min_dist = dist[v];
					vertex = v;
				}
			}
			if (vertex == std::numeric_limits<size_t>::max()) {
				break;
			}
			used[vertex] = true;
			for (const auto& it : edges(vertex)) {
				const size_t to = it.to();
				const T len = min_dist + it.weight();
				if (!used[to] && umin(dist[to], len)) {
					dist[to] = len;
					last[to] = it.id();
				}
			}
		}
		distance->swap(dist);
		if (last_edge != nullptr) {
			last_edge->swap(last);
		}
	}

	bool try_kuhn(const size_t vertex, std::vector<bool>& used, std::vector<size_t>& match) const;

};

template<typename T, size_t MASK>
void Graph<T, MASK>::clear() {
	vertices_count_ = 0;
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
	partition.assign(vertices_count_, kNone);
	Queue<size_t> q(vertices_count_);
	for (size_t i = 0; i < vertices_count_; ++i) {
		if (partition[i] == kNone) {
			q.clear();
			q.push(i);
			partition[i] = 0;
			while (!q.empty()) {
				const size_t vertex = q.pop_front();
				const size_t color = partition[vertex];
				for (const auto& it : edges(vertex)) {
					const size_t to = it.to();
					if (partition[to] == kNone) {
						partition[to] = color ^ 1;
						q.push(to);
						continue;
					}
					if (partition[to] == color) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

template<typename T, size_t MASK>
void Graph<T, MASK>::maximal_matching(std::vector<size_t>* vertex_match) const {
	std::vector<size_t> match(vertices_count_, std::numeric_limits<size_t>::max());
	std::vector<bool> matched(vertices_count_, false);
	for (const size_t v : vertices()) {
		for (const auto& it : edges(v)) {
			const size_t to = it.to();
			if (match[to] == std::numeric_limits<size_t>::max()) {
				match[to] = v;
				matched[v] = true;
				break;
			}
		}
	}
	std::vector<bool> used;
	for (const size_t v : vertices()) {
		if (matched[v]) {
			continue;
		}
		used.assign(vertices_count_, false);
		try_kuhn(v, used, match);
	}
	vertex_match->swap(match);
}

template<typename T, size_t MASK>
bool Graph<T, MASK>::try_kuhn(const size_t vertex, std::vector<bool>& used, std::vector<size_t>& match) const {
	used[vertex] = true;
	for (const auto& it : edges(vertex)) {
		const size_t to = it.to();
		if (match[to] == std::numeric_limits<size_t>::max()) {
			match[to] = vertex;
			return true;
		}
	}
	for (const auto& it : edges(vertex)) {
		const size_t to = it.to();
		if (!used[match[to]] && try_kuhn(match[to], used, match)) {
			match[to] = vertex;
			return true;
		}
	}
	return false;
}
