#include <iostream>
#include <queue>
#include <limits>
#include <vector>
#include <set>
#include <map>
#include "gh_tree.h"
#include "graph.h"

using Blob = std::set<int>;

GomoryHuTree::GomoryHuTree(const Graph& og_graph) {
    tree.assign(n, std::vector<edge>());
    std::vector<Blob> blobs;

    Graph blob_tree(n);

    // Initialize the first blob with all vertices
    Blob all_vertices;
    for (int i = 0; i < n; i++) {
        all_vertices.insert(i);
    }
    blobs.push_back({all_vertices});

    for (size_t i = 0; i < blobs.size(); ++i) {
        while (blobs[i].size() > 1) {
            std::vector<int> color(blobs.size(), 0);
            //map color -> vector<blob index> (blobs that share the color)
            std::map<int, std::vector<int>> mp;
            color[i] = -1;
            int next_color = 1;
            for (int j = 0; j < blobs.size(); ++j) {
                if (color[j] == 0) {
                    dfs(j, blob_tree.adj, mp, color, next_color);
                    next_color++;
                }
            }

            //stores vectors containing each vertex of a 'mega blob'
            std::vector<std::vector<int>> mega_blobs;

            //iterate through the map and create mega blobs
            for (auto [key, value] : mp) {
                std::vector<int> new_mega_blob;
                for (int k = 0; k < value.size(); k++) {
                    for (int vertex : blobs[value[k]]) {
                        new_mega_blob.push_back(vertex);
                    }
                }
                mega_blobs.push_back(new_mega_blob);
            }

            //map each vertex back to original vertex?
            std::map<int, int> perm;

            //in the blended graph, spaces [0, mega_blobs.size()] will represent the blobs, and the remaining will represent individual vertices
            Graph blended_graph(n);
            //iterate through megablobs and for each, create new 'mega edges' to all other blobs
            for (int j = 0; j < mega_blobs.size(); j++) {
                std::vector<int> edge_weights((int)mega_blobs.size());
                for (int vertex : mega_blobs[j]) {
                    for (auto og_edge : og_graph.adj[vertex]) {
                        edge_weights[og_edge.to] += og_edge.weight;
                    }
                }
                for (int k = 0; k < mega_blobs.size(); k++) {
                    if (k == j) continue;
                    blended_graph.add_edge(j, k, edge_weights[k]);
                    blended_graph.add_edge(k, j, edge_weights[k]);
                }
            }
            // //after splitting
            // for (blobs in our half) {
            //     insert into blobs[i] to replace
            // }
            //create new blob and insert vertices, append.
        }
    }
}

// GomoryHuTree::GomoryHuTree(const std::vector<std::vector<edge>>& graph) {
//     n = graph.size();
//     tree.assign(n, std::vector<edge>());
//     Blobs = set<int> // collection of vertices of graph
//     std::vector<Blobs> blobs; // stores vertices that are inside each of the blobs
// 	std::vector<std::vector<edge>> tree; // stores edges between blobs
//     blobs = {vertices(graph)}; 
//     for (size_t i = 0; i <= blobs.size(); ++i) {
//     	while (blob.size() >= 1) {
//     		... // treat this blob as X from the whiteboard
//     	} // outcome is that we get one extra blob, some edges of "tree" change
//     } //outcome is n blobs of size 1 and tree on blobs

//     for (vec in tree) {
//     	for (edge in vec) {
//     		GH_tree[blob[edge.from].front()].push_back(edge{blob[edge.from].front(), blob[edge.to].front(), 
//     																							edge.weight});
//     	}
//     }
//     return;
// }

// int n;
// std::vector<std::vector<edges>> GH_tree;

// // the ... part:
// // we have a graph on blob.size() vertices with edges written in tree
// vector<int> color(blob.size(), 0);
// color[i] = -1;
// int next_color = 1;
// for (int i = 0; i < blob.size(); ++i) {
// 	if (color[i] == 0) {
// 		dfs(vertex == i, color == next_color);
// 		next_color++;
// 	}
// } //all blobs are colored, the number of colors is next_color (+-1)
// // read about graph condensation/compression
// //new vector of blobs for components of blobs from the whiteboard
// //new grap: vertices - colors, weight on edges - sum of weights between the vertices from the blobs of said colors
// //add vertices from blob[i] to the set of vertices of the new graph, so now it has 
// //														next_color + blob[i].size() vertices
// //add edges from/to those vertices too

// // run min cut on this new graph with s-f being any two vertices of blob[i], flag = true

// //...

void GomoryHuTree::dfs(int blob, const std::vector<std::vector<edge>>& adj, std::map<int, std::vector<int>>& mp, std::vector<int>& color, int current_color) {
    color[blob] = current_color;
    mp[current_color].push_back(blob);
    for (edge neighbor : adj[blob]) {
        int from = neighbor.from, to = neighbor.to, weight = neighbor.weight;
        if (color[to] == 0) {
            dfs(to, adj, mp, color, current_color);
        }
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
