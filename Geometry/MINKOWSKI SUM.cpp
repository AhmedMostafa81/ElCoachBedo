#include <bits/stdc++.h>
using namespace std;

/* ====================================================================
 * MINKOWSKI SUM & DIFFERENCE TEMPLATE
 * Time Complexities:
 * - Minkowski Sum/Diff: O(N + M)
 * - Point in Convex Polygon: O(log(N + M))
 * - Minimum Distance between Polygons: O(N + M)
 * ==================================================================== */

struct Point {
    long long x, y;
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

// 2D Cross Product. Positive if b is strictly counter-clockwise from a.
long long cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

// 2D Dot Product. Used for distances and projections.
long long dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

// Reorders a convex polygon so the bottom-left point is at index 0.
// This guarantees the edges strictly increase in polar angle, starting from [0, pi].
void reorder(vector<Point>& P) {
    int pos = 0;
    for (int i = 1; i < P.size(); i++) {
        if (P[i].y < P[pos].y || (P[i].y == P[pos].y && P[i].x < P[pos].x)) {
            pos = i;
        }
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

// Computes A ⊕ B in strictly O(N + M) time using Two-Pointers.
vector<Point> minkowski_sum(vector<Point> A, vector<Point> B) {
    reorder(A); 
    reorder(B);
    
    int n = A.size(), m = B.size();
    // Append the first two elements to avoid modulo operations and handle wrap-around gracefully
    A.push_back(A[0]); A.push_back(A[1]);
    B.push_back(B[0]); B.push_back(B[1]);
    
    vector<Point> res;
    int i = 0, j = 0;
    
    // Merge the edges based on polar angles
    while (i < n || j < m) {
        res.push_back(A[i] + B[j]);
        
        Point vA = A[i + 1] - A[i];
        Point vB = B[j + 1] - B[j];
        
        if (i == n) { j++; continue; } // A is exhausted, push B
        if (j == m) { i++; continue; } // B is exhausted, push A
        
        long long cp = cross(vA, vB);
        if (cp > 0) i++;       // vA angle is smaller, advance A
        else if (cp < 0) j++;  // vB angle is smaller, advance B
        else { i++; j++; }     // Collinear, advance both
    }
    return res;
}

// Computes A ⊖ B (Minkowski Difference) in O(N + M) time.
vector<Point> minkowski_diff(vector<Point> A, vector<Point> B) {
    for (auto& p : B) {
        p.x = -p.x;
        p.y = -p.y;
    }
    // Negating all vertices inherently rotates B by 180 degrees.
    // It safely preserves the counter-clockwise order!
    return minkowski_sum(A, B);
}

// O(log K) Binary search to check if a point is inside a convex polygon
// Assumes poly is counter-clockwise and poly[0] is the bottom-left point
bool point_in_convex(const vector<Point>& poly, Point pt) {
    int n = poly.size();
    if (n < 3) return false; 
    
    // Check if point is outside the angular wedge of the polygon
    if (cross(poly[1] - poly[0], pt - poly[0]) < 0) return false;
    if (cross(poly[n - 1] - poly[0], pt - poly[0]) > 0) return false;
    
    // Binary search for the wedge containing the point
    int l = 1, r = n - 2;
    int ans = 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (cross(poly[mid] - poly[0], pt - poly[0]) >= 0) {
            ans = mid;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    // Check if the point is on the left side (or on) the closing segment of the wedge
    return cross(poly[ans + 1] - poly[ans], pt - poly[ans]) >= 0;
}

// O(1) Distance from point p to line segment a-b
double dist_to_segment(Point a, Point b, Point p) {
    Point ab = b - a;
    Point ap = p - a;
    Point bp = p - b;
    
    // Projection falls outside the segment, closer to 'a'
    if (dot(ab, ap) <= 0) return sqrt(dot(ap, ap));
    // Projection falls outside the segment, closer to 'b'
    if (dot(ab, bp) >= 0) return sqrt(dot(bp, bp));
    // Projection is inside the segment, use Area / Base
    return (double)abs(cross(ab, ap)) / sqrt(dot(ab, ab));
}

// ====================================================================
// SUBPROBLEM SOLVERS
// ====================================================================

// 1. COLLISION QUERY: Does A intersect with B shifted by vector v?
// Time: O(N+M) setup, O(log(N+M)) per query
bool check_collision(const vector<Point>& A, const vector<Point>& B, Point v) {
    vector<Point> diff_poly = minkowski_diff(A, B);
    return point_in_convex(diff_poly, v);
}

// 2. MINIMUM DISTANCE: Shortest exact distance between Polygon A and Polygon B
// Time: O(N+M)
double min_dist_polygons(vector<Point> A, vector<Point> B) {
    vector<Point> diff_poly = minkowski_diff(A, B);
    
    // If the origin is inside the Minkowski difference, they intersect. Distance is 0.
    if (point_in_convex(diff_poly, {0, 0})) {
        return 0.0;
    }
    
    // Otherwise, the distance is the shortest distance from the Origin (0,0) to the boundary
    double ans = 1e18;
    int k = diff_poly.size();
    for (int i = 0; i < k; i++) {
        ans = min(ans, dist_to_segment(diff_poly[i], diff_poly[(i + 1) % k], {0, 0}));
    }
    return ans;
}
