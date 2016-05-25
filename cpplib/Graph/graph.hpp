#pragma once
#include "Head.h"
#include "Maths.h"

class Graph {
public:
	enum Type {
		GT_WEIGHTED = 1,
		GT_FLOW = 2
	};

	int *from, *to, *parent;
	ll *weight;
	vvi edges;
	int maxVertexNum, maxEdgesNum;
	int vertexCount, edgesCount;

	Graph(const int maxVertexNum, const int maxEdgesNum, const int mask = 0) : maxVertexNum(maxVertexNum), maxEdgesNum(maxEdgesNum) {
		from = new int[maxEdgesNum];
		to = new int[maxEdgesNum];
		edges.resize(maxVertexNum);
		weight = (mask & GT_WEIGHTED ? new ll[maxEdgesNum] : NULL);
		vertexCount = 0;
		edgesCount = 0;
	}

	virtual ~Graph() {
		deepClear();
	}

	void initGraph(const int _vertexCount) {
		vertexCount = _vertexCount;
	}

	bool isSparse() const {
		return !vertexCount || sqr(static_cast<ll>(vertexCount)) >= (edgesCount << 4);
	}

	void clear(int size = -1) {
		edgesCount = 0;
		size = (size == -1 ? sz(edges) : std::min(size, sz(edges)));
		if (size == -1) {
			size = sz(edges);
		}
		for (int i = 0; i < size; ++i) {
			edges[i].clear();
		}
	}

	void deepClear() {
		maxVertexNum = 0;
		maxEdgesNum = 0;
		edgesCount = 0;
		edges.clear();
		if (from != NULL) {
			delete[] from;
		}
		if (to != NULL) {
			delete[] to;
		}
		if (weight != NULL) {
			delete[] weight;
		}
		if (parent != NULL) {
			delete[] parent;
		}
	}

	int addDirectedEdge(const int _from, const int _to) {
		from[edgesCount] = _from;
		to[edgesCount] = _to;
		edges[_from].emplace_back(edgesCount);
		return edgesCount++;
	}

	int addDirectedWeightedEdge(const int _from, const int _to, const ll _weight) {
		weight[edgesCount] = _weight;
		return addDirectedEdge(_from, _to);
	}

	bool isBipartite(int w[], int cnt[], int v, int col) const {
		int * vertices = new int[vertexCount];
		vertices[0] = v;
		w[v] = col;
		int head = 0, tail = 1;
		while (head < tail) {
			int curVertex = vertices[head++], curColor = w[curVertex];
			if (cnt != NULL) {
				++cnt[curColor];
			}
			for (auto& it : edges[curVertex]) {
				int toVertex = to[it];
				if (w[toVertex] == -1) {
					w[toVertex] = curColor ^ 1;
					vertices[tail++] = toVertex;
				}
				else
					if (w[toVertex] != (curColor ^ 1)) {
						delete[] vertices;
						return false;
					}
			}
		}
		delete[] vertices;
		return true;
	}

	bool isBipartite(int w[], int cnt[] = NULL) const {
		std::fill_n(w, vertexCount, -1);
		if (cnt != NULL) {
			cnt[0] = cnt[1] = 0;
		}
		bool ret = true;
		for (int i = 0; i < vertexCount && ret; ++i) {
			if (w[i] == -1) {
				ret &= isBipartite(w, cnt, i, 0);
			}
		}
		return ret;
	}

	bool topSortAcyclic(int order[]) const
		// non-recursive topological sorting, works only for acyclic graphs 
	{
		int * degree = new int[vertexCount];
		std::fill_n(degree, vertexCount, 0);
		for (int i = 0; i < edgesCount; ++i) {
			++degree[to[i]];
		}
		int head = 0, tail = 0;
		for (int i = 0; i < vertexCount; ++i) {
			if (!degree[i]) {
				order[tail++] = i;
			}
		}
		while (head < tail) {
			int curVertex = order[head++];
			for (auto& it : edges[curVertex]) {
				int toVertex = to[it];
				if (!(--degree[toVertex])) {
					order[tail++] = toVertex;
				}
			}
		}
		delete[] degree;
		return tail == vertexCount;
	}

	void topSortRec(int order[]) const {
		bool * used = new bool[vertexCount];
		std::fill_n(used, vertexCount, false);
		int cnt = 0;
		for (int i = 0; i < vertexCount; ++i) {
			if (!used[i]) {
				_topSortDfs(i, order, used, cnt);
			}
		}
		std::reverse(order, order + vertexCount);
	}

	void dijkstra(int startVertex, ll dist[], int last[] = NULL) const;
	ll dijkstra(int startVertex, int finishVertex, int path[] = NULL) const;

	size_t find_vertex_with_max_degree() const {
		size_t result = 0;
		size_t best = 0;
		for (int i = 0; i < vertexCount; ++i) {
			if (umax(best, edges[i].size())) {
				result = i;
			}
		}
		return result;
	}

	template<size_t N> void floyd(ll(&dist)[N][N]) const {
		for (int i = 0; i < vertexCount; ++i) {
			std::fill_n(dist[i], vertexCount, LINF);
			dist[i][i] = 0;
		}
		for (int i = 0; i < edgesCount; ++i) {
			ll& cur = dist[from[i]][to[i]];
			cur = min(cur, weight[i]);
		}
		for (int k = 0; k < vertexCount; ++k) {
			for (int i = 0; i < vertexCount; ++i) {
				for (int j = 0; j < vertexCount; ++j) {
					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}
	}

private:
	Graph();

	void _topSortDfs(const int v, int order[], bool used[], int& cnt) const
		// internal function for recursive topological sorting
	{
		used[v] = true;
		for (auto& it : edges[v]) {
			int toVertex = to[it];
			if (!used[toVertex]) {
				_topSortDfs(toVertex, order, used, cnt);
			}
		}
		order[cnt++] = v;
	}

};

class UndirectedGraph : public Graph {
public:

	UndirectedGraph(const int maxVertexNum, const int maxEdgesNum, const int mask = 0) : Graph(maxVertexNum, maxEdgesNum << 1, mask) {}

	void initGraph(const int _vertexCount) {
		vertexCount = _vertexCount;
	}

	void addBidirectionalEdge(const int _from, const int _to) {
		addDirectedEdge(_from, _to);
		addDirectedEdge(_to, _from);
	}

	void addBidirectionalWeightedEdge(const int _from, const int _to, const ll _weight) {
		addDirectedWeightedEdge(_from, _to, _weight);
		addDirectedWeightedEdge(_to, _from, _weight);
	}

private:
	UndirectedGraph();

};

void Graph::dijkstra(int startVertex, ll dist[], int last[]) const {
	std::fill_n(dist, vertexCount, LINF);
	dist[startVertex] = 0;
	if (last != NULL) {
		std::fill_n(last, vertexCount, -1);
	}
	if (isSparse()) {
		std::priority_queue<std::pair<ll, int>> q;
		q.push(mp(0, startVertex));
		while (!q.empty()) {
			auto cur_pair = q.top();
			q.pop();
			int curVertex = cur_pair.Y;
			ll curlen = -cur_pair.X;
			if (curlen > dist[curVertex]) {
				continue;
			}
			for (auto& it : edges[curVertex]) {
				ll newlen = curlen + weight[it];
				int toVertex = to[it];
				if (dist[toVertex] > newlen) {
					dist[toVertex] = newlen;
					q.push(mp(-newlen, toVertex));
					last[toVertex] = it;
				}
			}
		}
	}
	else {
		bool * used = new bool[vertexCount];
		std::fill_n(used, vertexCount, false);
		for (int i = 0; i < vertexCount; ++i) {
			int index = -1;
			ll curlen = LINF;
			for (int j = 0; j < vertexCount; ++j) {
				if (!used[j] && dist[j] < curlen) {
					curlen = dist[j];
					index = j;
				}
			}
			if (index == -1) {
				break;
			}
			used[index] = true;
			for (auto& it : edges[index]) {
				int toVertex = to[it];
				ll newlen = curlen + weight[it];
				if (!used[toVertex] && dist[toVertex] > newlen) {
					dist[toVertex] = newlen;
					last[toVertex] = it;
				}
			}
		}
		delete[] used;
	}
}

ll Graph::dijkstra(int startVertex, int finishVertex, int path[]) const {
	if (startVertex == finishVertex) {
		return 0;
	}
	ll * dist = new ll[vertexCount];
	int * last = NULL;
	if (path != NULL) {
		last = new int[vertexCount];
	}
	dijkstra(startVertex, dist, last);
	if (path != NULL) {
		std::fill_n(path, vertexCount, -1);
		int pathSize = 0, curVertex = finishVertex;
		while (curVertex != startVertex) {
			int curEdge = last[curVertex];
			path[pathSize++] = curEdge;
			curVertex = from[curEdge];
		}
		std::reverse(path, path + pathSize);
		delete[] last;
	}
	ll ans = dist[finishVertex];
	delete[] dist;
	return ans;
}

int stronglyConnectedComponents(const Graph& g, const Graph& gr, int color[])
// g - graph, gr - reversed graph, color - output (index of SCC for each vertex)
{
	int n = g.vertexCount;
	int * order = new int[n];
	bool * used = new bool[n];
	int * q = new int[n];
	std::fill_n(order, n, 0);
	g.topSortRec(order);
	int cnt = 0;
	std::fill_n(used, n, false);
	for (int i = 0; i < n; ++i) {
		int v = order[i];
		if (!used[v]) {
			int head = 0, tail = 1;
			q[0] = v;
			while (head < tail) {
				int curVertex = q[head++];
				color[curVertex] = cnt;
				used[curVertex] = true;
				for (auto& it : gr.edges[curVertex]) {
					int to = gr.to[it];
					if (!used[to]) {
						used[to] = true;
						q[tail++] = to;
					}
				}
			}
			++cnt;
		}
	}
	delete[] q;
	delete[] used;
	delete[] order;
	return cnt;
}