#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int size)
    : adjacencyMatrix(size, std::vector<int>(size, 0)) {}

MatrixGraph::MatrixGraph(const IGraph& graph) {
    adjacencyMatrix.resize(graph.VerticesCount(),
                           std::vector<int>(graph.VerticesCount(), 0));
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j = 0; j < nextVertices.size(); ++j) {
            adjacencyMatrix[i][nextVertices[j]] = 1;
        }
    }
}

MatrixGraph::~MatrixGraph() {}

void MatrixGraph::AddEdge(int from, int to) { adjacencyMatrix[from][to] = 1; }

int MatrixGraph::VerticesCount() const { return (int)adjacencyMatrix.size(); }

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> NextVertices;
    for (int j = 0; j < adjacencyMatrix.size(); ++j) {
        if (adjacencyMatrix[vertex][j] != 0) {
            NextVertices.push_back(j);
        }
    }
    return NextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (int from = 0; from < adjacencyMatrix.size(); ++from) {
        if (adjacencyMatrix[from][vertex] == 1) {
            prevVertices.push_back(from);
        }
    }

    return prevVertices;
}