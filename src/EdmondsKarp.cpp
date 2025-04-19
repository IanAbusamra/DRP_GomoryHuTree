#include "EdmondsKarp.h"
#include <queue>
#include <algorithm>

EdmondsKarp::EdmondsKarp(int _n)
  : n(_n),
    capacity(_n, std::vector<int>(_n, 0)),
    adj(_n)
{}

void EdmondsKarp::addEdge(int u, int v, int cap) {
    capacity[u][v] = cap;
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int EdmondsKarp::bfs(int s, int t, std::vector<int>& parent) {
    std::fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    std::queue<std::pair<int,int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        auto [u, flow] = q.front(); 
        q.pop();
        for (int v : adj[u]) {
            if (parent[v] == -1 && capacity[u][v] > 0) {
                parent[v] = u;
                int new_flow = std::min(flow, capacity[u][v]);
                if (v == t) return new_flow;
                q.push({v, new_flow});
            }
        }
    }
    return 0;
}

int EdmondsKarp::maxflow(int s, int t) {
    int flow = 0;
    std::vector<int> parent(n);
    int new_flow;
    while ((new_flow = bfs(s, t, parent)) != 0) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }
    return flow;
}

std::vector<bool> EdmondsKarp::minCutSide(int s) {
    std::vector<bool> vis(n, false);
    std::queue<int> q;
    vis[s] = true; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!vis[v] && capacity[u][v] > 0) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    return vis;
}
