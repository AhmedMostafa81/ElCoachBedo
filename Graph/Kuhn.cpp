//  maximum bipartite matching == minimum vertex cover(minimum number of nodes that cover all edges)
//minimum vertex cover (the complement of maximum independent set)



#include <bits/stdc++.h>
using namespace std;

struct Kuhn { // O(V * E)
    int n, m;                       // left: 0..n-1, right: 0..m-1
    vector<vector<int>> adj;        // adj[v] = list of right nodes
    vector<int> matchR;             // matchR[to] = matched left node or -1
    vector<bool> used;

    Kuhn(int _n, int _m) : n(_n), m(_m) {
        adj.assign(n, {});
        matchR.assign(m, -1);
        used.assign(n, false);
    }

    void addEdge(int u, int v) {    // u in [0..n-1], v in [0..m-1]
        adj[u].push_back(v);
    }

    bool dfs(int v) {
        if (used[v]) return false;
        used[v] = true;
        for (int to : adj[v]) {
            if (matchR[to] == -1 || dfs(matchR[to])) {
                matchR[to] = v;
                return true;
            }
        }
        return false;
    }

    int maximumMatching() {
        int match = 0;
        for (int v = 0; v < n; ++v) {
            fill(used.begin(), used.end(), false);
            if (dfs(v)) ++match;
        }
        return match;
    }

    vector<pair<int,int>> getMatchingPairs() {
        vector<pair<int,int>> res;
        for (int v = 0; v < m; ++v)
            if (matchR[v] != -1)
                res.emplace_back(matchR[v], v);
        return res;
    }

    pair<vector<int>,vector<int>> minVertexCover() {
    vector<int> matchL(n, -1);
    for (int r = 0; r < m; r++)
        if (matchR[r] != -1)
            matchL[matchR[r]] = r;

    vector<int> visL(n, 0), visR(m, 0);
    queue<int> q;

    // start from free left vertices
    for (int i = 0; i < n; i++) {
        if (matchL[i] == -1) {
            visL[i] = 1;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (int v : adj[u]) {
            if (v == matchL[u]) continue;   // skip matched edge L->R
            if (visR[v]) continue;

            visR[v] = 1;

            if (matchR[v] != -1 && !visL[matchR[v]]) {
                visL[matchR[v]] = 1;
                q.push(matchR[v]);
            }
        }
    }

    vector<int> leftCover, rightCover;

    for (int i = 0; i < n; i++)
        if (!visL[i]) leftCover.push_back(i);

    for (int i = 0; i < m; i++)
        if (visR[i]) rightCover.push_back(i);

    return {leftCover, rightCover};
}

};
