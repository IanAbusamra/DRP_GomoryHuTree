#include <vector>
#include "edge.h"

struct Graph {
    int n;
    std::vector<std::vector<edge>> adj;

    Graph(int n) : n(n), adj(n) {}

    void add_edge(int from, int to, int start) {
        adj[from].emplace_back(from, to, start);
        adj[to].emplace_back(to, from, start);
    }
};