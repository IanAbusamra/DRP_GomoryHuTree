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
            std::vector<std::vector<int>> mega_blobs; // DS: store indices of blobs instead of just vertices 

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

            //in the blended graph, spaces [0, mega_blobs.size()] will represent the blobs, and the remaining will represent individual vertices
            Graph blended_graph(n); //DS: blod[i].size() + mp.size()
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
                    blended_graph.add_edge(j, k, edge_weights[k]); //DS: double counting
                    //blended_graph.add_edge(k, j, edge_weights[k]); //DS: double counting
                }
            }

            //iterate through the vertices of the current blob and add edges to each other vertex to the blended graph
            // for (int vertex : blobs[i]) { // DS: elements of blobs[i] are up to n. Instead you should just index with i
            //     for (auto og_edge : og_graph.adj[vertex]) {
            //         blended_graph.add_edge(mega_blobs.size() + vertex, mega_blobs.size() + og_edge.to, og_edge.weight);
            //     }
            // }
            for (std::set<int>::iterator it = blobs[i].begin(); it != blobs[i].end(); ++it) { // DS: because blobs[i] is a set you have to use iterators
                auto vertex = *it;
                for (auto og_edge : og_graph.adj[vertex]) {
                    auto to = og_edge.to;
                    blended_graph.add_edge(mega_blobs.size() + vertex, mega_blobs.size() + to, og_graph.adj[vertex][to].weight);
                }
            }

            //iterate through the vertices of the current blob and add 'mega edges' from those vertices to the mega blobs
            for (std::set<int>::iterator it = blobs[i].begin(); it != blobs[i].end(); ++it) { //DS: the same issue with vertex being up to n
                auto vertex = *it;
                for (int j = 0; j < mega_blobs.size(); j++) {
                    int sum = 0;
                    for (int k = 0; k < mega_blobs[j].size(); k++) {
                        //need to implement below function
                        //sum += find_dist(vertex, mega_blobs[j][k]);
                        continue;
                    }
                    // blended_graph.add_edge(mega_blobs.size() + vertex, j, sum);
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
