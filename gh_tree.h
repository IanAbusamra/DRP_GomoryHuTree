#ifndef GH_TREE_H
#define GH_TREE_H

#include <vector>
#include <set>
#include <map>
#include "graph.h"

class GomoryHuTree {
private:
    int n;

    // Each node in the tree has a list of edges (from Gomory-Hu)
    std::vector<std::vector<int>> tree;
    
    // Internal DFS function used for component coloring
    void dfs(int vertex, const std::vector<std::vector<int>>& adj, std::vector<int>& color, int current_color);

    std::pair<int, std::vector<int>> GomoryHuTree::max_flow(Graph blended_graph, int size, bool flag = true);

public:
    GomoryHuTree(const Graph& og_graph);

    int query(int u, int v, bool include_vertices = false);

};

#endif // GH_TREE_H
