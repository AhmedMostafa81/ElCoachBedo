const int INF = 2;

// Finds solution for a linear system of equations mod p (p is prime)
// Complexity: O(min(n, m) * n * m + min(n, m) * log(mod))
// Returns:
//   0   -> No solution
//   1   -> Unique solution (stored in ans)
//   INF -> Multiple solutions (one valid sample in ans; freeVariableCnt = number of free variables)
//          Total solutions mod M (when mod is prime): fastPow(mod % M, freeVariableCnt, M)
int freeVariableCnt = 0;

// DO NOT CALL WITH EMPTY a
int gauss_mod(vector<vector<long long>> a, vector<long long>& ans, long long mod) {
    int n = static_cast<int>(a.size());
    int m = static_cast<int>(a[0].size()) - 1;
    freeVariableCnt = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= m; ++j) {
            a[i][j] = (a[i][j] % mod + mod) % mod;
        }
    }

    vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        int sel = -1;
        for (int i = row; i < n; ++i) {
            if (a[i][col] != 0) {
                sel = i;
                break;
            }
        }
        if (sel == -1) {
            continue;
        }
        swap(a[sel], a[row]);
        where[col] = row;

        long long inv_pivot = fastPow(a[row][col], mod - 2, mod);
        for (int j = col; j <= m; ++j) {
            a[row][j] = static_cast<long long>(((__int128)a[row][j] * inv_pivot) % mod);
        }

        for (int i = 0; i < n; ++i) {
            if (i != row && a[i][col] != 0) {
                long long factor = a[i][col];
                for (int j = col; j <= m; ++j) {
                    long long prod = static_cast<long long>(((__int128)factor * a[row][j]) % mod);
                    long long val = a[i][j] - prod;
                    if (val < 0) val += mod;
                    a[i][j] = val;
                }
            }
        }
        ++row;
    }

    ans.assign(m, 0);
    for (int i = 0; i < m; ++i) {
        if (where[i] != -1) {
            ans[i] = a[where[i]][m];
        }
    }

    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        for (int j = 0; j < m; ++j) {
            sum = static_cast<long long>((sum + (__int128)ans[j] * a[i][j]) % mod);
        }
        if (sum != a[i][m]) {
            freeVariableCnt = 0;
            return 0;
        }
    }

    for (int i = 0; i < m; ++i) {
        if (where[i] == -1) {
            freeVariableCnt++;
        }
    }
    return (freeVariableCnt > 0) ? INF : 1;
}
