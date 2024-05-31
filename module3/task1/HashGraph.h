#ifndef HASH_GRAPH
#define HASH_GRAPH

#include "IGraph.h"

#include <unordered_set>

struct HashGraph : public IGraph {
   public:
    HashGraph(int size);

    HashGraph(const IGraph& graph);

    ~HashGraph();

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::unordered_set<int>> adjacencyMap;
};

#endif