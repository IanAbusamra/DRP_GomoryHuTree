#ifndef GH_TREE_H
#define GH_TREE_H

#include <vector>
#include <set>
#include <map>
#include "graph.h"

class GomoryHuTree {
private:
    int n;

    std::vector<std::vector<int>> tree;
    std::vector<Blob> blobs;
    
    // Internal DFS function used for component coloring
    void dfs(int vertex, const std::vector<std::vector<int>>& adj, std::vector<int>& color, int current_color);

    std::pair<int, std::vector<int>> max_flow(Graph blended_graph, int size);

public:
    GomoryHuTree(const Graph& og_graph);

    int query(int u, int v, bool include_vertices = false);

};

#endif // GH_TREE_H
