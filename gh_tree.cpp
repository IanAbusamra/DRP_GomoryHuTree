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

            //iterate through the vertices of the current blob and add edges to each other vertex to the blended graph
            for (int vertex : blobs[i]) {
                for (auto og_edge : og_graph.adj[vertex]) {
                    blended_graph.add_edge(mega_blobs.size() + vertex, mega_blobs.size() + og_edge.to, og_edge.weight);
                }
            }

            //iterate through the vertices of the current blob and add 'mega edges' from those vertices to the mega blobs
            for (int vertex : blobs[i]) {
                for (int j = 0; j < mega_blobs.size(); j++) {
                    int sum = 0;
                    for (int k = 0; k < mega_blobs[j].size(); k++) {
                        //need to implement below function
                        //sum += find_dist(vertex, mega_blobs[j][k]);
                    }
                    blended_graph.add_edge(mega_blobs.size() + vertex, j, sum);
                    blended_graph.add_edge(j, mega_blobs.size() + vertex, sum);
                }
            }

            //max_flow(mega_blobs.size(), mega_blobs.size() + 1) -> (mincut value, mincut edges)
            Blob half_1, half_2;
            for (e : mincut_edges) {
                half_1.insert(e.from);
                half_2.insert(e.to);
            }
            blobs[i] = half_1;
            blobs.push_back(half_2);
        }
    }
}

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
