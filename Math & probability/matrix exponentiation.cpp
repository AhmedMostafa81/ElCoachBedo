#include <bits/stdc++.h>
#define matrix std::vector<std::vector<long long>>
#define zero(n, m) matrix(n, std::vector<long long>(m))
const int mod = 1e9+7;

matrix mul(matrix &a, matrix &b){
    int n = a.size(), m = b[0].size(), l = b.size();
    assert(l == a[0].size()); 
    
    matrix res = zero(n, m);
    for(int i=0; i<n; ++i)
        for(int j=0; j<m; ++j)
            for(int k=0; k<l; ++k) 
                res[i][j] = (res[i][j] + a[i][k]*b[k][j]) % mod;
    return res;
}

matrix matrix_fp(matrix &b, long long e){
    if(e == 1) return b;
    matrix res = matrix_fp(b, e>>1);
    res = mul(res, res);
    if(e&1) res = mul(res, b);
    return res;
}

void base_case(matrix &dp){
    // HOW TO SET THE BASE CASE:
    // 'dp' is typically an (N x 1) column vector storing the earliest known values of your sequence.
    // The size N depends on the order of your recurrence (e.g., Fibonacci relies on the last 2 terms, so N=2).
    // 
    // Example for standard Fibonacci: F(0) = 0, F(1) = 1.
    // The vector is [ F(0) ]
    //               [ F(1) ]
    dp[0][0] = 0; // Represents F(0)
    dp[1][0] = 1; // Represents F(1)
}

void transition(matrix &T){
    // HOW TO SET THE TRANSITION MATRIX:
    // 'T' is an (N x N) matrix that shifts the state forward by exactly 1 step.
    // The goal is to set up T such that: T * [ F(i)   ]  =  [ F(i+1) ]
    //                                        [ F(i+1) ]     [ F(i+2) ]
    //
    // Row 0 calculates the new top element (F(i+1)):
    // We want F(i+1) = 0*F(i) + 1*F(i+1). 
    // So the coefficients for row 0 are {0, 1}.
    T[0][0] = 0; 
    T[0][1] = 1; 

    // Row 1 calculates the new bottom element (F(i+2)):
    // We want F(i+2) = 1*F(i) + 1*F(i+1).
    // So the coefficients for row 1 are {1, 1}.
    T[1][0] = 1;
    T[1][1] = 1;
}

signed main() {
    std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
    long long n;
    std::cin >> n;
    if(!n) std::cout << n << '\n';
    else{
        matrix T = zero(2, 2);
        matrix dp = zero(2, 1);
        base_case(dp);
        transition(T);
        T = matrix_fp(T, n);
        std::cout << mul(T, dp)[0][0] << '\n';
    }
}
