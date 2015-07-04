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

	Graph(const int maxVertexNum, const int maxEdgesNum, const int mask = 0);

	virtual ~Graph() {
		deepClear();
	}

	void initGraph(const int _vertexCount) {
		vertexCount = _vertexCount;
	}

	bool isSparse() const {
		return !vertexCount || sqr(static_cast<ll>(vertexCount)) >= (edgesCount << 4);
	}

	void clear(int size = -1);
	void deepClear();

	int addDirectedEdge(const int _from, const int _to);
	int addDirectedWeightedEdge(const int _from, const int _to, const ll _weight);

	bool isBipartite(int w[], int cnt[], int v, int col) const;
	bool isBipartite(int w[], int cnt[] = NULL) const;
	bool topSortAcyclic(int order[]) const;
	void topSortRec(int order[]) const;
	void dijkstra(int startVertex, ll dist[]) const;

private:
	Graph();
	void _topSortDfs(const int v, int order[], bool used[], int& cnt) const;

};

class UndirectedGraph : public Graph {
public:

	UndirectedGraph(const int maxVertexNum, const int maxEdgesNum, const int mask = 0) : Graph(maxVertexNum, maxEdgesNum << 1, mask) {}

	void initGraph(const int _vertexCount) {
		vertexCount = _vertexCount;
	}

	void addBidirectionalEdge(const int _from, const int _to);

private:
	UndirectedGraph();

};

void stronglyConnectedComponents(const Graph& g, const Graph& gr, int color[]);