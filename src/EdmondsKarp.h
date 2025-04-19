#ifndef EDMONDS_KARP_H
#define EDMONDS_KARP_H

#include <vector>
#include <limits>

static constexpr int INF = std::numeric_limits<int>::max();

struct EdmondsKarp {
    EdmondsKarp(int n);

    void addEdge(int u, int v, int cap);

    int maxflow(int s, int t);

    std::vector<bool> minCutSide(int s);

private:
    int n;
    std::vector<std::vector<int>> capacity;
    std::vector<std::vector<int>> adj;

    int bfs(int s, int t, std::vector<int>& parent);
};

#endif // EDMONDS_KARP_H
