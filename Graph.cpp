#include "Graph.h"

Graph::Graph(const int maxVertexNum, const int maxEdgesNum, const int mask) : maxVertexNum(maxVertexNum), maxEdgesNum(maxEdgesNum) {
	from = new int[maxEdgesNum];
	to = new int[maxEdgesNum];
	edges.resize(maxVertexNum);
	weight = (mask & (1 << GT_WEIGHTED) ? new ll[maxEdgesNum] : NULL);
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
