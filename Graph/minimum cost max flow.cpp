//    ->     max flow == min cut (edges have cost and i want to get minimum cost to disconnect src and sink)
// maximum edge disjoint path (no two paths share the same edge)
// maximum independent path (no two paths share the same edge or the same node) (add constraint on vertex)

// minimum path cover (minimum number of paths that covers all graph without sharing edges) (DAG)
//   = n - max_matching



//    minimum cost max flow

#include <bits/stdc++.h>
using namespace std;

struct FlowEdge {
    int v, u;
    long long cap, flow = 0, cost;

    FlowEdge(int v, int u, long long cap, long long cost)
        : v(v), u(u), cap(cap), cost(cost) {}
};

struct FlowNetwork {
    const long long INF = 1e18;

    int n, m = 0, s, t;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    vector<int> level, ptr;
    queue<int> q;

    FlowNetwork(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, long long cap, long long cost = 0) {
        edges.emplace_back(v, u, cap, cost);
        edges.emplace_back(u, v, 0, -cost);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }

    // Dinic BFS
    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        q = queue<int>();
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap - edges[id].flow < 1) continue;
                if (level[edges[id].u] != -1) continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    // Dinic DFS
    long long dfs(int v, long long pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;

        for (int &cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;

            if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                continue;

            long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0) continue;

            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long max_flow() {
        long long f = 0;
        while (true) {
            if (!bfs()) break;
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, INF)) {
                f += pushed;
            }
        }
        return f;
    }

    // Min Cost Flow of K units using SPFA
    long long min_cost_flow(long long K) {
        long long flow = 0, cost = 0;
        vector<long long> dist(n);
        vector<int> in_queue(n), parent(n), parent_edge(n);

        while (flow < K) {
            fill(dist.begin(), dist.end(), INF);
            fill(in_queue.begin(), in_queue.end(), 0);
            fill(parent.begin(), parent.end(), -1);
            fill(parent_edge.begin(), parent_edge.end(), -1);

            dist[s] = 0;
            queue<int> q;
            q.push(s);
            in_queue[s] = 1;

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                in_queue[v] = 0;

                for (int id : adj[v]) {
                    int u = edges[id].u;
                    if (edges[id].cap - edges[id].flow < 1) continue;

                    if (dist[u] > dist[v] + edges[id].cost) {
                        dist[u] = dist[v] + edges[id].cost;
                        parent[u] = v;
                        parent_edge[u] = id;
                        if (!in_queue[u]) {
                            in_queue[u] = 1;
                            q.push(u);
                        }
                    }
                }
            }

            if (dist[t] == INF) break;

            long long push = K - flow;
            int v = t;
            while (v != s) {
                int id = parent_edge[v];
                push = min(push, edges[id].cap - edges[id].flow);
                v = parent[v];
            }

            flow += push;
            cost += push * dist[t];

            v = t;
            while (v != s) {
                int id = parent_edge[v];
                edges[id].flow += push;
                edges[id ^ 1].flow -= push;
                v = parent[v];
            }
        }

        return (flow < K ? -1 : cost);
    }

    // Returns all original forward edges that carry positive flow.
    // Each tuple = {from, to, used_flow, cost}
    vector<tuple<int, int, long long, long long>> get_used_edges() const {
        vector<tuple<int, int, long long, long long>> res;
        for (int i = 0; i < m; i += 2) {
            const FlowEdge &e = edges[i];   // only forward/original edges
            if (e.cap > 0 && e.flow > 0) {
                res.emplace_back(e.v, e.u, e.flow, e.cost);
            }
        }
        return res;
    }
};
