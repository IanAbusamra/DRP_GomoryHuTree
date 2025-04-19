#ifndef GRAPH_H
#define GRAPH_H
#include <vector>

struct Graph {
    int n;
    std::vector<std::vector<int>> adj;

    Graph(int n) : n(n), adj(n + 1, std::vector<int>(n + 1, -1)) {};

    void add_edge(int from, int to, int weight) {
        adj[from][to] = weight;
        adj[to][from] = weight;
    }

    void increase_edge(int from, int to, int weight) {
        adj[from][to] += weight;
        adj[to][from] += weight;
    }
};

#endif