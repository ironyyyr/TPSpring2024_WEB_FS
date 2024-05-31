#include "ListGraph.h"

ListGraph::ListGraph(int size) : adjacencyList(size) {}

ListGraph::ListGraph(const IGraph& graph) {
    adjacencyList.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjacencyList[i] = graph.GetNextVertices(i);
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) { adjacencyList[from].push_back(to); }

int ListGraph::VerticesCount() const { return (int)adjacencyList.size(); }

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    return adjacencyList[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (int from = 0; from < adjacencyList.size(); ++from) {
        for (int to : adjacencyList[from]) {
            if (to == vertex) {
                prevVertices.push_back(from);
            }
        }
    }
    return prevVertices;
}