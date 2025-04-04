#include <vector>
#include <iostream>
#include <queue>
#include <limits>
#include "edge.h"

class GomoryHuTree {
private:
    int n;
    std::vector<std::vector<edge>> tree;
    
public:
    GomoryHuTree(const std::vector<std::vector<edge>>& graph);

    int query(int u, int v, bool include_vertices = false);
};