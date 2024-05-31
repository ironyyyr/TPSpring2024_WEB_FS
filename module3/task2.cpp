/*
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших
путей между какими-то вершинами. Найдите количество различных кратчайших путей
между заданными вершинами.
*/

#include <climits>
#include <iostream>
#include <queue>
#include <vector>

struct ListGraph {
   public:
    ListGraph(int size) : adjacencyList(size) {}

    ~ListGraph() {}

    void AddEdge(int from, int to) { adjacencyList[from].push_back(to); }

    int VerticesCount() const { return (int)adjacencyList.size(); }

    std::vector<int> GetNextVertices(int vertex) const {
        return adjacencyList[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const {
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

   private:
    std::vector<std::vector<int>> adjacencyList;
};

void upgradeBFS(const ListGraph& listGraph, int vertex,
                std::vector<int>& lengthOfTheWay,
                std::vector<int>& numOfShortedWays) {
    std::queue<int> tmpQueue;
    tmpQueue.push(vertex);
    lengthOfTheWay[vertex] = 0;
    numOfShortedWays[vertex] = 1;

    while (!tmpQueue.empty()) {
        int currVertex = tmpQueue.front();
        tmpQueue.pop();

        for (int nextVertex : listGraph.GetNextVertices(currVertex)) {
            if (lengthOfTheWay[nextVertex] > lengthOfTheWay[currVertex] + 1) {
                lengthOfTheWay[nextVertex] = lengthOfTheWay[currVertex] + 1;
                numOfShortedWays[nextVertex] = numOfShortedWays[currVertex];
                tmpQueue.push(nextVertex);
            } else if (lengthOfTheWay[currVertex] + 1 ==
                       lengthOfTheWay[nextVertex]) {
                numOfShortedWays[nextVertex] += numOfShortedWays[currVertex];
            }
        }
    }
}

int findShortestPaths(const ListGraph& listGraph, int firstVertex,
                      int secondVertex) {
    std::vector<int> lengthOfTheWay(listGraph.VerticesCount(), INT_MAX);
    std::vector<int> numOfShortedWays(listGraph.VerticesCount(), 0);

    upgradeBFS(listGraph, firstVertex, lengthOfTheWay, numOfShortedWays);

    return numOfShortedWays[secondVertex];
}

int main() {
    int numOfVertices = 0;
    int numOfEdges = 0;
    std::cin >> numOfVertices >> numOfEdges;

    ListGraph listGraph(numOfVertices);
    int first, second;

    for (int i = 0; i < numOfEdges; ++i) {
        std::cin >> first >> second;
        listGraph.AddEdge(first, second);
        listGraph.AddEdge(second, first);
    }

    std::cin >> first >> second;

    int result = findShortestPaths(listGraph, first, second);
    std::cout << result << std::endl;

    return 0;
}
