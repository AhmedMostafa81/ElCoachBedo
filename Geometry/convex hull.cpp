template<class T>
vector<Point<T>> convex_hull(vector<Point<T>> points){
    sort(points.begin(), points.end(), [](const Point<T>& p1, const Point<T>& p2){
        if(p1.X == p2.X) return p1.Y < p2.Y;
        return p1.X < p2.X;
    });
    points.erase(unique(points.begin(), points.end()), points.end());
    if((int)points.size() < 3) return points;
    vector<Point<T>> hull;
    for(int i=0; i<(int)points.size(); i++){
        while((int)hull.size() > 1 && sgn(cross(hull.back()-hull[(int)hull.size()-2], points[i]-hull[(int)hull.size()-2])) <= 0){
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    int lowerLen = (int)hull.size();
    for(int i=(int)points.size()-2; i>=0; i--){
        while((int)hull.size() > lowerLen && sgn(cross(hull.back()-hull[(int)hull.size()-2], points[i]-hull[(int)hull.size()-2])) <= 0){
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    hull.pop_back();
    return hull;
}
