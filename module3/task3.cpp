/*
Требуется отыскать самый выгодный маршрут между городами.
*/

#include <climits>
#include <iostream>
#include <set>
#include <vector>

struct MyComparator {
    bool operator()(const std::pair<int, int>& first,
                    const std::pair<int, int>& second) const {
        if (first.first != second.first) {
            return first.first < second.first;
        }
        return first.second < second.second;
    }
};

struct ListGraph {
   public:
    ListGraph(int size) : adjacencyList(size) {}

    ~ListGraph() {}

    void AddEdge(int from, int weight, int to) {
        adjacencyList[from].push_back(std::make_pair(weight, to));
    }

    int VerticesCount() const { return (int)adjacencyList.size(); }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const {
        return adjacencyList[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const {
        std::vector<int> prevVertices;
        for (int from = 0; from < adjacencyList.size(); ++from) {
            for (std::pair<int, int> to : adjacencyList[from]) {
                if (to.first == vertex) {
                    prevVertices.push_back(from);
                }
            }
        }
        return prevVertices;
    }

   private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;
};

void diikstraSearch(const ListGraph& listGraph, int vertex,
                    std::vector<int>& lenghtOfTheWay,
                    std::vector<int>& parents) {
    std::set<std::pair<int, int>, MyComparator> prioritySet;

    prioritySet.insert(std::make_pair(0, vertex));
    parents[vertex] = -1;
    lenghtOfTheWay[vertex] = 0;

    while (!prioritySet.empty()) {
        int currVertex = prioritySet.begin()->second;
        int currDist = prioritySet.begin()->first;

        prioritySet.erase(*prioritySet.begin());
        for (std::pair<int, int> nextVerticePair :
             listGraph.GetNextVertices(currVertex)) {
            int nextVertice = nextVerticePair.second;
            int nextWeight = nextVerticePair.first;

            if (lenghtOfTheWay[nextVertice] >
                lenghtOfTheWay[currVertex] + nextWeight) {
                prioritySet.erase(
                    std::make_pair(lenghtOfTheWay[nextVertice], nextVertice));
                lenghtOfTheWay[nextVertice] =
                    lenghtOfTheWay[currVertex] + nextWeight;
                parents[nextVertice] = currVertex;
                prioritySet.insert(
                    std::make_pair(lenghtOfTheWay[nextVertice], nextVertice));
            } else if (lenghtOfTheWay[nextVertice] == INT_MAX) {
                prioritySet.insert(nextVerticePair);
                lenghtOfTheWay[nextVertice] =
                    lenghtOfTheWay[currVertex] + nextWeight;
                parents[nextVertice] = currVertex;
            }
        }
    }
}

int mainDiisktra(const ListGraph& listGraph, int firstElem, int secondElem) {
    std::vector<int> lengthOfTheWay(listGraph.VerticesCount(), INT_MAX);
    std::vector<int> parents(listGraph.VerticesCount(), 0);

    diikstraSearch(listGraph, firstElem, lengthOfTheWay, parents);

    return lengthOfTheWay[secondElem];
}

int main() {
    int numOfVertices = 0, numOfEdges = 0;
    std::cin >> numOfVertices >> numOfEdges;

    ListGraph listGraph(numOfVertices);

    for (int i = 0; i < numOfEdges; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        listGraph.AddEdge(from, weight, to);
        listGraph.AddEdge(to, weight, from);
    }

    int first, second, result;

    std::cin >> first >> second;

    result = mainDiisktra(listGraph, first, second);

    std::cout << result;

    return 0;
}