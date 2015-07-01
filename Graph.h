#pragma once
#include "Head.h"

struct Graph {

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
		weight = (mask & (1 << GT_WEIGHTED) ? new ll[maxEdgesNum] : NULL);
		vertexCount = 0;
		edgesCount = 0;
	}

	virtual ~Graph() {
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

	void initGraph(const int _vertexCount) {
		vertexCount = _vertexCount;
	}

	void clear(int size = -1) {
		edgesCount = 0;
		size = (size == -1 ? sz(edges) : min(size, sz(edges)));
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
		edges[_from].push_back(edgesCount);
		return edgesCount++;
	}

private:
	Graph();

};

struct UndirectedGraph : Graph {

	UndirectedGraph(const int maxVertexNum, const int maxEdgesNum, const int mask = 0) : Graph(maxVertexNum, maxEdgesNum << 1, mask) {}

	void initGraph(const int _vertexCount) {
		vertexCount = _vertexCount;
	}

	void addBidirectionalEdge(const int _from, const int _to) {
		addDirectedEdge(_from, _to);
		addDirectedEdge(_to, _from);
	}

private:
	UndirectedGraph();

};