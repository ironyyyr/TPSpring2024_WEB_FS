/*
Необходимо написать несколько реализаций интерфейса:
ListGraph, хранящий граф в виде массива списков смежности,
MatrixGraph, хранящий граф в виде матрицы смежности,
SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев
поиска, ArcGraph, хранящий граф в виде одного массива пар {from, to}. Также
необходимо реализовать конструктор, принимающий const IGraph&. Такой конструктор
должен скопировать переданный граф в создаваемый объект. Для каждого класса
создавайте отдельные h и cpp файлы. Число вершин графа задается в конструкторе
каждой реализации.
*/

#include <deque>
#include <functional>
#include <iostream>
#include <queue>

#include "ArcGraph.h"
#include "HashGraph.h"
#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"

void BFS(const IGraph& graph, int vertex, std::vector<bool>& visited,
         const std::function<void(int)>& func) {
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph& graph, const std::function<void(int)>& func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph& graph, int vertex, std::vector<bool>& visited,
         const std::function<void(int)>& func) {
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void mainDFS(const IGraph& graph, const std::function<void(int)>& func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            DFS(graph, i, visited, func);
        }
    }
}

void topologicalSortInternal(const IGraph& graph, int vertex,
                             std::vector<bool>& visited,
                             std::deque<int>& sorted) {
    visited[vertex] = true;

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            topologicalSortInternal(graph, nextVertex, visited, sorted);
        }
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph& graph) {
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            topologicalSortInternal(graph, i, visited, sorted);
        }
    }

    return sorted;
}

int main() {
    ListGraph listGraph1(7);

    listGraph1.AddEdge(0, 1);
    listGraph1.AddEdge(0, 5);
    listGraph1.AddEdge(1, 2);
    listGraph1.AddEdge(1, 3);
    listGraph1.AddEdge(1, 5);
    listGraph1.AddEdge(1, 6);
    listGraph1.AddEdge(3, 2);
    listGraph1.AddEdge(3, 4);
    listGraph1.AddEdge(3, 6);
    listGraph1.AddEdge(5, 4);
    listGraph1.AddEdge(5, 6);
    listGraph1.AddEdge(6, 4);

    MatrixGraph matrixGraph1(listGraph1);
    HashGraph hashGraph1(matrixGraph1);
    ArcGraph arcGraph1(hashGraph1);

    mainBFS(listGraph1, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainBFS(matrixGraph1, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainBFS(hashGraph1, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainBFS(arcGraph1, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    ArcGraph arcGraph2(arcGraph1);
    HashGraph hashGraph2(arcGraph2);
    MatrixGraph matrixGraph2(hashGraph2);
    ListGraph listGraph2(matrixGraph2);
    std::cout << std::endl;

    mainDFS(arcGraph2, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(hashGraph2, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(matrixGraph2, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(listGraph2, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << std::endl;

    for (int vertex : topologicalSort(listGraph1)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    for (int vertex : topologicalSort(matrixGraph2)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    for (int vertex : topologicalSort(hashGraph1)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    for (int vertex : topologicalSort(arcGraph2)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
}