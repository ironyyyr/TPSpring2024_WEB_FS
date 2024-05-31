#include "ArcGraph.h"

ArcGraph::ArcGraph(int size) : verticesCount(size) {}

ArcGraph::ArcGraph(const IGraph& graph) {
    verticesCount = graph.VerticesCount();
    for (int i = 0; i < verticesCount; ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j = 0; j < nextVertices.size(); ++j) {
            adjacencyArc.push_back(std::make_pair(i, nextVertices[j]));
        }
    }
}

ArcGraph::~ArcGraph() {}

void ArcGraph::AddEdge(int from, int to) {
    adjacencyArc.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const { return verticesCount; }

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertices;
    for (std::pair<int, int> from : adjacencyArc) {
        if (from.first == vertex) {
            nextVertices.push_back(from.second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;
    for (std::pair<int, int> from : adjacencyArc) {
        if (from.second == vertex) {
            prevVertices.push_back(from.first);
        }
    }
    return prevVertices;
}