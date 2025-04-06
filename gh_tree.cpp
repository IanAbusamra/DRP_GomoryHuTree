#include <iostream>
#include <queue>
#include <limits>
#include <vector>
#include "gh_tree.h"

GomoryHuTree::GomoryHuTree(const std::vector<std::vector<edge>>& graph) {
    n = graph.size();
    tree.assign(n, std::vector<edge>());

    for (int u = 1; u < n; ++u) {
        int min_cut_value = 42;

        tree[u].push_back(edge(u, u - 1, min_cut_value));
        tree[u - 1].push_back(edge(u - 1, u, min_cut_value));
    }
}

int GomoryHuTree::query(int u, int v, bool include_vertices) {
    int ans = -1;

    if (include_vertices) {
        std::cout << "Include Vertices Flag Set." << std::endl;
        std::cout << "Querying min-cut between " << u << " and " << v << std::endl;
    }

    std::cout << "Minimum cut between " << u << " and " << v << ": " << ans << std::endl;
    
    return ans;
}
