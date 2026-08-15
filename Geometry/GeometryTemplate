#include <bits/stdc++.h>

using namespace std;


typedef long long ll;
typedef long double ld;

template<class T>

using Point = complex<T>;
using pt = Point<long long>;
using fpt = Point<long double>;

#define X real()
#define Y imag()



const ld PI = acos(-1.0L);
const ld EPS = 1e-12;

int sgn(ll val){
    if(val < 0) return -1;
    if(val > 0) return 1;
    else return 0;
}
int sgn(ld val){
    if(val <= -EPS) return -1;
    if(val >= EPS) return 1;
    return 0;
}

template<class T>
ld mag(Point<T> p){
    return hypot((ld)p.X, (ld)p.Y);
}

template<class T>
ld dir(Point<T> p){
    return atan2((ld)p.Y, (ld)p.X);
}

template<class T>
T dot(Point<T> p1, Point<T> p2){
    return (conj(p1)*p2).X;
}

template<class T>
T cross(Point<T> p1, Point<T> p2){
    return (conj(p1)*p2).Y;
}


// returns rotation of P1 around P2, by angle ang (in radians);
fpt rotate(fpt p1, fpt p2, ld ang){
    p1 -= p2;
    p1 *= polar((ld)1, ang);
    p1 += p2;
    return p1;
}

// return true if points arg is in the range [pi, 0)

template<class T>
bool half(Point<T> p){
    return (sgn(p.Y) > 0 || (sgn(p.Y) == 0 && sgn(p.X) < 0));
}

// sorts points by arg from ]-pi, pi];

template<class T>
bool polar_cmp(Point<T>& p1, Point<T>& p2){
    if(half(p1) != half(p2)) return half(p1) < half(p2);
    return sgn(cross(p1, p2)) > 0;
}


// gives you two points from the line ax + by + c = 0
pair<fpt, fpt> line_to_points(ld a, ld b, ld c){
    ld norm = a*a + b*b;
    fpt p1(-a*c/norm, -b*c/norm);
    fpt p2 = p1 + fpt(-b, a);
    return {p1, p2};
}

template<class T>
ld dist_point_line(Point<T> p, Point<T> a, Point<T> b){
    return abs(cross(b-a, p-a))/mag(b-a);
}

template<class T>
ld dist_point_segment(Point<T> p, Point<T> a, Point<T> b){
    if(sgn(dot(p-a, b-a)) <= 0){
        return mag(p-a);
    }
    if(sgn(dot(p-b, a-b)) <= 0){
        return mag(p-b);
    }
    return dist_point_line(p, a, b);
}


// ENSURE THEY ARE NOT PARALLEL FIRST
template<class T> 
fpt line_intersection(Point<T> a, Point<T> b, Point<T> c, Point<T> d){
    T den = cross(b-a, d-c);
    T num = cross(c-a, d-c);
    ld t = (ld)num/den;

    ld x = a.X + t*(b.X-a.X);
    ld y = a.Y + t*(b.Y-a.Y);
    return fpt(x, y);
}

// check if p is on a b

template<class T>
bool on_line(Point<T> p, Point<T> a, Point<T> b){
    return sgn(cross(p-a, b-a)) == 0;

}

template<class T>
bool on_ray(Point<T> p, Point<T> a, Point<T> b){
    return on_line(p, a, b) && sgn(dot(p-a, b-a)) >= 0;
}

template<class T>
bool on_segment(Point<T> p, Point<T> a, Point<T> b){
    return on_ray(p, a, b) && on_ray(p, b, a);
}

template<class T>
bool check_segment_intersection(Point<T> p1, Point<T> p2, Point<T> p3, Point<T> p4){ 
    if(on_segment(p3, p1, p2) || on_segment(p4, p1, p2) || on_segment(p1, p3, p4) || on_segment(p2, p3, p4)){
        return true;
    }
    if(sgn(cross(p2-p1, p4-p1))*sgn(cross(p2-p1, p3-p1)) < 0 &&
       sgn(cross(p3-p4, p1-p4))*sgn(cross(p3-p4, p2-p4)) < 0){
        return true;
    }
    return false;
}


template<class T>
Point<T> perp(Point<T> p){
    return {-p.Y, p.X};
}

template<class T>
fpt projection_point_line(Point<T> p, Point<T> a, Point<T> b){
    T num = dot(b-a, p-a);
    T den = dot(b-a, b-a);
    ld t = (ld)num/den;
    ld x = a.X + t*(b.X-a.X);
    ld y = a.Y + t*(b.Y-a.Y);
    return fpt(x, y);
}

fpt circumcenter(fpt p1, fpt p2, fpt p3){
    // MAKE SURE ALL 3 POINTS ARE NOT COLLINEAR (use on_line())
    fpt m1 = (p1 + p2)/2.0L;
    fpt m2 = (p2 + p3)/2.0L;
    fpt v1 = perp(p1-p2), v2 = perp(p3-p2);
    return line_intersection(m1, m1 + v1, m2, m2 + v2);

}


template<class T>
vector<fpt> line_circle_intersection(Point<T> c, T r, Point<T> a, Point<T> b){
    fpt p = projection_point_line(c, a, b);
    ld d2 = norm(p-c);
    ld r2 = (ld)r*r;
    if(sgn(d2-r2) > 0){
        return {};
    }
    if(sgn(d2-r2) == 0){
        return {p};
    }
    ld x = sqrt(r2 - d2);
    fpt uv = (a-b)/mag(a-b);
    return {p + uv*x, p - uv*x};
}


vector<fpt> circle_circle_intersection(fpt c1, ld r1, fpt c2, ld r2) {
    if (sgn(mag(c1 - c2)) == 0 && sgn(r1 - r2) == 0) {
        if (sgn(r1) == 0) return {c1};
        return vector<fpt>(3);
    }  
    ld dist = mag(c1 - c2); 
    if (sgn(dist - (r1 + r2)) == 1 || sgn(dist - abs(r1 - r2)) == -1) {
        return {};
    }
    ld cos_val = (r1 * r1 - r2 * r2 + dist * dist) / (2 * r1 * dist); 
    cos_val = max(-1.0L, min(1.0L, cos_val)); 
    ld ang = acos(cos_val); 
    fpt base = c1 + ((c2 - c1) / dist) * r1;
    if (sgn(ang) == 0) {
        return {base};
    } 
    else{
        return {rotate(base, c1, ang),rotate(base, c1, -ang)};
    }
}
 
template<class T>
T polygon_2area(vector<Point<T>> &a){
    T ans = 0;
    int sz = (int)a.size();
    for(int i=0; i<sz; ++i){
        int j = (i == sz-1 ? 0:i+1);
        ans += cross(a[i], a[j]);
    }
    return std::abs(ans);
}


// returns {internal points, boundary points}
// it uses pick's theorem which states : Area = I + (B/2) - 1;

array<ll,2> latice_points_in_polygon(vector<pt> &a){
    int sz = (int)a.size();
    ll B = 0;
    for(int i=0; i<sz; i++){
        int j = (i == sz-1 ? 0:i+1); 
        B += gcd(abs(a[j].X-a[i].X), abs(a[j].Y-a[i].Y));
    }
    ll twoArea = polygon_2area(a);
    ll I = (twoArea-B+2)/2;
    return {I, B};
}

template<class T>
fpt polygon_centroid(const vector<Point<T>>& a) {
    int sz = (int)a.size();
    ld x = 0, y = 0, area2 = 0;
    for (int i = 0; i < sz; i++) {
        int j = (i == sz-1 ? 0:i+1);
        // Let cross product be evaluated as fpt
        ld c = cross(a[i], a[j]);  
        x += c * (a[i].X + a[j].X); 
        y += c * (a[i].Y + a[j].Y);
        area2 += c;
    }
    return {x / (3.0L * area2), y / (3.0L * area2)};
}

// -1 outside
// 0 on border
// 1 inside

template<class T>
int point_in_polygon(Point<T> p, vector<Point<T>>& a){
    int n = (int)a.size();
    for(int i=0; i<n; i++){
        Point<T> p1 = a[i], p2 = (a[(i == n-1 ? 0:i+1)]);
        if(on_segment(p, p1, p2)) return 0;
    }
    int cnt = 0;
    for(int i=0; i<n; i++){
        Point<T> p1 = a[i], p2 = (a[(i == n-1 ? 0:i+1)]);
        if(cross(p2-p1, {1, 0}) == 0) continue;
        if(p1.Y < p2.Y) swap(p1, p2);
        if(p.Y >= p2.Y && p.Y <= p1.Y && sgn(cross(p1-p, p2-p)) <= 0){
            // then we cross segment
            if(p1.Y == p.Y) continue;
            cnt++;
        }
    }
    return ((cnt&1) ? 1:-1);
}

// EVERYTHING ABOVE HERE SHOULD BE FINE



ld triangleAreaUsingSides(ld a, ld b, ld c){
    // finds the triangle 
    ld s = (a + b + c)/2;
    ld res = s*(s-a)*(s-b)*(s-c);
    return sqrt(res);
}
ld triangleAreaUsingMedians(ld a, ld b, ld c){
    return 4.0L/3 * triangleAreaUsingSides(a, b, c);
}


const int mxN=1e5, MOD=998244353;

void go() {

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int tc=1;
    // cin >> tc;
    while(tc--){
        go();
    }




}
