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
            for (std::set<int>::iterator it = blobs[i].begin(); it != blobs[i].end(); ++it) { //DS: the same issue with vertex being up to n
                int vertex = index_map[*it];
                for (int j = 0; j < mega_blobs.size(); j++) {
                    int sum = 0;
                    for (int k = 0; k < mega_blobs[j].size(); k++) {
                        //need to implement below function
                        //sum += find_dist(vertex, mega_blobs[j][k]);
                        continue;
                    }
                    blended_graph.add_edge(j, mega_blobs.size() + vertex, sum);
                }
            }

            //max_flow(blended_graph, mega_blobs.size(), mega_blobs.size() + 1, flag = true) -> (mincut value, connected component of the vertex mega_blobs.size() - a vector of vertices)
            //vertices (the og_graph vertices) of the blobs[i] in the connected component are indexed by mega_blobs.size()+ and correspond to 
            //                                                                                                          blobs[i][index - mega_blobs.size()] in the og_graph
            auto max_f = max_flow(blended_graph, mega_blobs.size(), true);
            // create a new blob out of those vertices of og_graph -> blobs.push_back(new blob{connectged component});
            // remove elements of the newly added blob blobs.push_back(new blob{connectged component}) from the blobs[i]
            
            //connect blobs[i] and the newly added blob blobs.push_back(new blob{connectged component})

            //for mega_blobs of the connected component in the blendnded graph are indexed by < bega_blods.size()
                //find the_blob that is connectd to blobs[i] in the blod_tree graph
                //disconnect it from the blobs[i] and connect it to newly added blod blobs.push_back(new blob{connectged component})
        }
    }
}

std::pair<int, std::vector<int>> GomoryHuTree::max_flow(Graph blended_graph, int size, bool flag) {
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
    int ans = -1;

    if (include_vertices) {
        std::cout << "Include Vertices Flag Set." << std::endl;
        std::cout << "Querying min-cut between " << u << " and " << v << std::endl;
    }

    std::cout << "Minimum cut between " << u << " and " << v << ": " << ans << std::endl;
    
    return ans;
}
