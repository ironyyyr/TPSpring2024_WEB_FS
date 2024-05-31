#include "HashGraph.h"

HashGraph::HashGraph(int size) : adjacencyMap(size) {}

HashGraph::HashGraph(const IGraph& graph) {
    adjacencyMap.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j = nextVertices.size() - 1; j >= 0; --j) {
            adjacencyMap[i].insert(nextVertices[j]);
        }
    }
}

HashGraph::~HashGraph() {}

void HashGraph::AddEdge(int from, int to) { adjacencyMap[from].insert(to); };

int HashGraph::VerticesCount() const { return (int)adjacencyMap.size(); };

std::vector<int> HashGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertives;
    for (int to : adjacencyMap[vertex]) {
        nextVertives.push_back(to);
    }
    return nextVertives;
};

std::vector<int> HashGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;
    for (int from = 0; from < adjacencyMap.size(); ++from) {
        for (int to : adjacencyMap[from]) {
            if (to == vertex) {
                prevVertices.push_back(from);
            }
        }
    }
    return prevVertices;
}