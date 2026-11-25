template<class T>
vector<Point<T>> convex_hull(vector<Point<T>> points){
    sort(points.begin(), points.end(), [](const Point<T>& p1, const Point<T>& p2){
        if(p1.X == p2.X) return p1.Y < p2.Y;
        return p1.X < p2.X;
    });
    points.erase(unique(points.begin(), points.end()), points.end());
    if((int)points.size() < 3) return points;
    vector<Point<T>> hull;

    for (int _=0; _<2; ++_) {
        int lowerLen = _ ? hull.size() : 1;
        for(int i=0; i<(int)points.size(); i++){
            while((int)hull.size() > lowerLen && sgn(cross(hull.back()-hull[(int)hull.size()-2], points[i]-hull[(int)hull.size()-2])) <= 0){
                hull.pop_back();
            }
            hull.push_back(points[i]);
        }
        reverse(points.begin(), points.end());
    }
    hull.pop_back();
    return hull;
}
