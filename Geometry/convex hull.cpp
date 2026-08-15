// Sardina
// Time Complexity: O(N * log(N))
template<class T>
bool clockwise(Point<T> a, Point<T> b, Point<T> c, bool include_collinear) {
    int o = orient(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}

template<class T>
void convex_hull(vector<Point<T>>& a, bool include_collinear = false) {
    Point<T> p0 = *min_element(a.begin(), a.end(), [](Point<T> a, Point<T> b) {
            return make_pair(a.Y, a.X) < make_pair(b.Y, b.X);
        });
    sort(a.begin(), a.end(), [&p0](const Point<T>& a, const Point<T>& b) {
            int o = orient(p0, a, b);
            if (o == 0)
                return (p0.X-a.X)*(p0.X-a.X) + (p0.Y-a.Y)*(p0.Y-a.Y)
                < (p0.X-b.X)*(p0.X-b.X) + (p0.Y-b.Y)*(p0.Y-b.Y);
            return o < 0;
        });
    if (include_collinear) {
        int i = (int)a.size()-1;
        while (i >= 0 && orient(p0, a[i], a.back()) == 0) i--;
        reverse(a.begin()+i+1, a.end());
    }
    vector<Point<T>> st;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !clockwise(st[st.size()-2], st.back(), a[i], include_collinear))
            st.pop_back();
        st.push_back(a[i]);
    }
    if (include_collinear == false && st.size() == 2 && st[0] == st[1])
        st.pop_back();
    a = st;
}
