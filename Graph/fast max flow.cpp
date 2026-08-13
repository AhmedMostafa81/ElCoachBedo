//    ->     max flow == min cut (edges have cost and i want to get minimum cost to disconnect src and sink)
// maximum edge disjoint path (no two paths share the same edge)
// maximum independent path (no two paths share the same edge or the same node) (add constraint on vertex)

// minimum path cover (minimum number of paths that covers all graph without sharing edges) (DAG)
//   = n - max_matching





//    fast max flow

struct FlowEdge { // O(E * sqrt(V))
    int v, u;
    long long cap, flow = 0;
    FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
    const long long flow_inf = 1e18;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, m = 0;
    int s, t;
    vector<int> level, ptr;
    queue<int> q;

    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, long long cap) {
        edges.emplace_back(v, u, cap);
        edges.emplace_back(u, v, 0);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }

    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap == edges[id].flow)
                    continue;
                if (level[edges[id].u] != -1)
                    continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    long long dfs(int v, long long pushed) {
        if (pushed == 0)
            return 0;
        if (v == t)
            return pushed;
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u])
                continue;
            long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long flow() {
        long long f = 0;
        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs())
                break;
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, flow_inf)) {
                f += pushed;
            }
        }
        return f;
    }

    vector<vector<int>> extract_flow_paths(long long flow_limit = -1) {
    vector<vector<int>> paths;

    while (true) {
        vector<int> path = {s};
        vector<bool> visited(n, false);
        long long pushed = flow_inf;

        bool found = false;
        function<bool(int)> dfs = [&](int v) -> bool {
            if (v == t) return true;
            visited[v] = true;
            for (int& i = ptr[v]; i < (int)adj[v].size(); ++i) {
                int id = adj[v][i];
                FlowEdge& e = edges[id];
                if (e.flow > 0 && !visited[e.u]) {
                    path.push_back(e.u);
                    long long minflow = min(pushed, e.flow);
                    pushed = minflow;
                    if (dfs(e.u)) {
                        e.flow -= pushed;
                        edges[id ^ 1].flow += pushed;
                        return true;
                    }
                    path.pop_back();
                }
            }
            return false;
        };

        fill(ptr.begin(), ptr.end(), 0);
        if (!dfs(s)) break;

        if (flow_limit != -1 && pushed > flow_limit) pushed = flow_limit;
        paths.push_back(path);
        if (flow_limit != -1) {
            flow_limit -= pushed;
            if (flow_limit <= 0) break;
        }
    }

    return paths;
    }


    vector<tuple<int, int, long long>> get_used_edges() {
        vector<tuple<int, int, long long>> result;
        for (const auto& e : edges) {
            if (e.cap > 0 && e.flow > 0) {
                result.emplace_back(e.v, e.u, e.flow);
            }
        }
        return result;
    }

    vector<pair<int, int>> min_cut() {
        // Step 1: Find reachable nodes from s in residual graph
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(s);
        vis[s] = true;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int id : adj[v]) {
                int u = edges[id].u;
                if (!vis[u] && edges[id].flow < edges[id].cap) {
                    vis[u] = true;
                    q.push(u);
                }
            }
        }

        // Step 2: Find saturated forward edges from reachable to unreachable
        vector<pair<int, int>> cut_edges;
        for (int i = 0; i < (int)edges.size(); i += 2) {
            const FlowEdge& e = edges[i];
            if (e.cap == e.flow && e.cap > 0 && vis[e.v] && !vis[e.u]) {
                cut_edges.emplace_back(e.v, e.u);
            }
        }

        return cut_edges;
    }

};

