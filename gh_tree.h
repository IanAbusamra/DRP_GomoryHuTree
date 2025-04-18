#ifndef GH_TREE_H
#define GH_TREE_H

#include <vector>
#include <set>
#include <map>
#include "edge.h"
#include "graph.h"

class GomoryHuTree {
private:
    int n;

    // Each node in the tree has a list of edges (from Gomory-Hu)
    std::vector<std::vector<edge>> tree;
    

    // Internal DFS function used for component coloring
    void dfs(int vertex, const std::vector<std::vector<edge>>& adj, std::map<int, std::vector<int>>& mp, std::vector<int>& color, int current_color);

    // Internal function used to find the max flow between two vertices
    std::pair<int, std::vector<int>> GomoryHuTree::max_flow(Graph blended_graph, int size, bool flag = true);

public:
    // Constructor: builds the Gomory-Hu tree from a graph (adjacency list)
    GomoryHuTree(const Graph& og_graph);

    // Query min-cut between two vertices, optionally printing extra info
    int query(int u, int v, bool include_vertices = false);

};

#endif // GH_TREE_H
