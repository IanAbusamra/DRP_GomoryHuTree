#include <iostream>
#include <queue>
#include <limits>
#include <climits>
#include <vector>
#include <set>
#include <map>
#include <function>
#include "gh_tree.h"
#include "graph.h"

using Blob = std::set<int>;

GomoryHuTree::GomoryHuTree(const Graph& og_graph) {
    blobs.clear();
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
            color[i] = -1;
            int next_color = 1;
            for (int j = 0; j < blobs.size(); ++j) {
                if (color[j] == 0) {
                    dfs(j, blob_tree.adj, color, next_color);
                    next_color++;
                }
            }

            //map each color to its megablobs
            std::map<int, std::vector<int>> mp;
            for (int j = 0; j < blobs.size(); j++) {
                mp[color[j]].push_back(j);
            }

            //stores vectors containing each blob index of a 'mega blob'
            std::vector<std::vector<int>> mega_blobs;

            //iterate through the map and create mega blobs
            for (const auto& [clr, mg_blob] : mp) {
                mega_blobs.push_back(mg_blob);
            }

            //in the blended graph, spaces [0, mega_blobs.size()] will represent the blobs, and the remaining will represent individual vertices
            Graph blended_graph(blobs[i].size() + mp.size());
            //iterate through megablobs and for each, create new 'mega edges' to all other blobs
            for (int j = 0; j < mega_blobs.size(); j++) {
                std::vector<int> edge_weights((int)mega_blobs.size());
                for (int vertex : mega_blobs[j]) {
                    for (int to : og_graph.adj[vertex]) {
                        if (to == -1) continue;
                        edge_weights[to] += og_graph.adj[vertex][to];
                    }
                }
                for (int k = 0; k < mega_blobs.size(); k++) {
                    if (k == j) continue;
                    blended_graph.add_edge(j, k, edge_weights[k]);
                }
            }

            //TODO: add +mega_blobs.size() in this map here
            std::vector<int> local_vertices(blobs[i].begin(), blobs[i].end());
            std::map<int,int> index_map; //map to compress the blob's vertices
            for (int idx = 0; idx < (int)local_vertices.size(); ++idx) {
                index_map[local_vertices[idx]] = idx;
            }

            //iterate through the vertices of the current blob and add edges to each other vertex to the blended graph
            for (std::set<int>::iterator it = blobs[i].begin(); it != blobs[i].end(); ++it) {
                int vertex = index_map[*it]; 
                for (int to : og_graph.adj[vertex]) {
                    if (to == -1) continue;
                    blended_graph.add_edge(mega_blobs.size() + vertex, mega_blobs.size() + index_map[to], og_graph.adj[vertex][to]);
                }
            }

            //iterate through the vertices of the current blob and add 'mega edges' from those vertices to the mega blobs
            for (std::set<int>::iterator it = blobs[i].begin(); it != blobs[i].end(); ++it) {
                int vertex = index_map[*it];
                for (int j = 0; j < mega_blobs.size(); j++) {
                    for (int k = 0; k < mega_blobs[j].size(); k++) {
                        if (og_graph.adj[*it][mega_blobs[j][k]] != -1) {
                            blended_graph.increase_edge(vertex, j, og_graph.adj[*it][mega_blobs[j][k]]);
                            blended_graph.increase_edge(j, vertex, og_graph.adj[*it][mega_blobs[j][k]]);
                        }
                    }
                }
            }

            auto [flow_val, reachable] = max_flow(blended_graph, mega_blobs.size());

            //create a new blob with the reachable vertices
            Blob new_blob;
            for (int idx = 0; idx < local_vertices.size(); ++idx) {
                if (reachable[mega_blobs.size() + idx]) {
                    new_blob.insert(local_vertices[idx]);
                }
            }

            //remove the vertices of the new blob from blobs[i]
            for (int v : new_blob) {
                blobs[i].erase(v);
            }

            //add edge between the split blob
            blobs.push_back(new_blob);
            int new_idx = blobs.size() - 1;

            blob_tree.add_edge(i, new_idx, flow_val);
        }
    }
}

std::pair<int, std::vector<int>> GomoryHuTree::max_flow(Graph blended_graph, int size) {
    std::pair<int, std::vector<int>> max_f;
    return max_f;
}

void GomoryHuTree::dfs(int blob, const std::vector<std::vector<int>>& adj, std::vector<int>& color, int current_color) {
    color[blob] = current_color;
    for (int neighbor : adj[blob]) {
        if (adj[blob][neighbor] != -1 && color[neighbor] == 0) {
            dfs(neighbor, adj, color, current_color);
        }
    }
}

int GomoryHuTree::query(int u, int v, bool include_vertices) {
    //possibly preprocess this mapping
    int idx_u = -1, idx_v = -1;
    for (int i = 0; i < (int)blobs.size(); ++i) {
        if (blobs[i].count(u)) idx_u = i;
        if (blobs[i].count(v)) idx_v = i;
        if (idx_u != -1 && idx_v != -1) break;
    }
    if (idx_u < 0 || idx_v < 0) {
        std::cerr << "Error: one of the vertices is not in any blob.\n";
        return -1;
    }

    int min_edge = INT_MAX;
    std::vector<bool> seen(blobs.size(), false);
    std::function<bool(int)> dfs = [&](int cur) -> bool {
        if (cur == idx_v) return true;
        seen[cur] = true;
        for (int nbr = 0; nbr < (int)blob_tree.adj[cur].size(); ++nbr) {
            int w = blob_tree.adj[cur][nbr];
            if (w < 0 || seen[nbr]) continue;
            int prev_min = min_edge;
            min_edge = std::min(min_edge, w);
            if (dfs(nbr)) return true;
            min_edge = prev_min;
        }
        return false;
    };

    dfs(idx_u);
    if (min_edge == INT_MAX) {
        std::cerr << "Error: no path between blobs " 
                  << idx_u << " and " << idx_v << "\n";
        return -1;
    }

    std::cout << "Min-cut between " << u << " and " << v << ": " << ans << std::endl;
    
    return ans;
}

// class GomoryHuTree {
//     public:
//         GomoryHuTree(const Graph& g);
//         int query(int u, int v, bool include_vertices);
    
//     private:
//         void initialize();
//         void split_blob(int blob_idx);
//         std::pair<Graph, std::map<int,int>> build_blended_graph(const Blob& blob, const std::vector<std::vector<int>>& mega_blobs);
//         std::pair<int, std::vector<int>> max_flow(Graph& g, int source, int target);
//         void dfs(int blob, const std::vector<std::vector<int>>& adj, std::vector<int>& color, int current_color);
    
//         std::vector<Blob> blobs;
//         Graph blob_tree;
//         const Graph& original_graph;
//     };