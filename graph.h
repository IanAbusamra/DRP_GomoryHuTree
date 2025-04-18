#include <vector>
#include "edge.h"

struct Graph {
    int n;
    std::vector<std::vector<edge>> adj;

    Graph(int n) : n(n), adj(n) {}

    void add_edge(int from, int to, int weight) {
        adj[from].emplace_back(from, to, weight);
        adj[to].emplace_back(to, from, weight);
    }
};