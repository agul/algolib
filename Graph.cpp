#include "Graph.h"

Graph::Graph(const int maxVertexNum, const int maxEdgesNum, const int mask) : maxVertexNum(maxVertexNum), maxEdgesNum(maxEdgesNum) {
	from = new int[maxEdgesNum];
	to = new int[maxEdgesNum];
	edges.resize(maxVertexNum);
	weight = (mask & GT_WEIGHTED ? new ll[maxEdgesNum] : NULL);
	vertexCount = 0;
	edgesCount = 0;
}

void Graph::clear(int size) {
	edgesCount = 0;
	size = (size == -1 ? sz(edges) : min(size, sz(edges)));
	if (size == -1) {
		size = sz(edges);
	}
	for (int i = 0; i < size; ++i) {
		edges[i].clear();
	}
}

void Graph::deepClear() {
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

int Graph::addDirectedEdge(const int _from, const int _to) {
	from[edgesCount] = _from;
	to[edgesCount] = _to;
	edges[_from].push_back(edgesCount);
	return edgesCount++;
}

int Graph::addDirectedWeightedEdge(const int _from, const int _to, const ll _weight) {
	weight[edgesCount] = _weight;
	return addDirectedEdge(_from, _to);
}

bool Graph::isBipartite(int w[], int cnt[], int v, int col) const {
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

bool Graph::isBipartite(int w[], int cnt[]) const {
	fill_n(w, vertexCount, -1);
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

bool Graph::topSortAcyclic(int order[]) const 
// non-recursive topological sorting, works only for acyclic graphs 
{
	int * degree = new int[vertexCount];
	fill_n(degree, vertexCount, 0);
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

void Graph::topSortRec(int order[]) const {
	bool * used = new bool[vertexCount];
	fill_n(used, vertexCount, false);
	int cnt = 0;
	for (int i = 0; i < vertexCount; ++i) {
		if (!used[i]) {
			_topSortDfs(i, order, used, cnt);
		}
	}
	reverse(order, order + vertexCount);
}

void Graph::_topSortDfs(const int v, int order[], bool used[], int& cnt) const 
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

void Graph::dijkstra(int startVertex, ll dist[]) const {
	fill_n(dist, vertexCount, LINF);
	dist[startVertex] = 0;
	if (isSparse()) {
		priority_queue<pair<ll, int>> q;
		q.push(mp(0, startVertex));
		while (!q.empty()) {
			auto it = q.top();
			q.pop();
			int curVertex = it.Y;
			ll curlen = -it.X;
			if (curlen > dist[curVertex]) {
				continue;
			}
			for (auto& it : edges[curVertex]) {
				ll newlen = curlen + weight[it];
				int toVertex = to[it];
				if (dist[toVertex] > newlen) {
					dist[toVertex] = newlen;
					q.push(mp(-newlen, toVertex));
				}
			}
		}
	} else {
		bool * used = new bool[vertexCount];
		fill_n(used, vertexCount, false);
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
				}
			}
		}
		delete[] used;
	}
}



void UndirectedGraph::addBidirectionalEdge(const int _from, const int _to) {
	addDirectedEdge(_from, _to);
	addDirectedEdge(_to, _from);
}



void stronglyConnectedComponents(const Graph& g, const Graph& gr, int color[])
// g - graph, gr - reversed graph, color - output (index of SCC for each vertex)
{
	int n = g.vertexCount;
	int * order = new int[n];
	bool * used = new bool[n];
	int * q = new int[n];
	fill_n(order, n, 0);
	g.topSortRec(order);
	int cnt = 0;
	fill_n(used, n, false);
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
}
