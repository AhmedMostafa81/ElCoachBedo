

struct Tag {
    // Identity tags (make sure when those are applied nothing changes)
    ll add = 0;
    
    bool is_empty() const {
        return add == 0;
    }
    void apply(const Tag &t) {
        add += t.add;
    }
};

struct Info {
    // Identity elements (make sure when this is merged it causes no issues)
    ll sum = 0; 
    ll len = 0;
    
    void apply(const Tag &t) {
        sum += t.add * len;
    }
};

Info operator+(const Info &a, const Info &b) {
    return {a.sum + b.sum, a.len + b.len};
}




template<class Info, class Tag>
struct LazySegmentTree {
    int n;
    vector<Info> info;
    vector<Tag> tag;
    
    void init(int sz){
        n = sz;
        info.assign(4*n, Info());
        tag.assign(4*n, Tag());
        return;
    }

    void build(vector<Info>& init, int x, int lx, int rx){
        tag[x] = Tag();
        if(rx-lx == 1){
            info[x] = init[lx];
            return;
        }
        int m = (rx + lx)/2;
        build(init, 2*x, lx, m);
        build(init, 2*x+1, m, rx);
        pull(x);
    }
    void build(vector<Info>& init){
        build(init, 1, 0, n);
    }
    
    void pull(int x) {
        info[x] = info[2 * x] + info[2 * x + 1];
    }
    
    void apply(int x, Tag t) {
        info[x].apply(t);
        tag[x].apply(t);
    }
    
    void push(int x) {
        if(tag[x].is_empty()) return;
        apply(2 * x, tag[x]);
        apply(2 * x + 1, tag[x]);
        tag[x] = Tag(); 
    }
    
    void rangeApply(int l, int r, Tag t, int x, int lx, int rx) {
        if (l >= rx || r <= lx || l >= r) return;
        if (lx >= l && rx <= r) {
            apply(x, t);
            return;
        }
        int m = (lx + rx) / 2;
        push(x);

        rangeApply(l, r, t, 2*x, lx, m);
        rangeApply(l, r, t, 2*x+1, m, rx);
        pull(x);
    }
    
    void rangeApply(int l, int r, Tag t) {
        rangeApply(l, r, t, 1, 0, n);
    }
    
    Info rangeQuery(int l, int r, int x, int lx, int rx) {
        if (l >= rx || r <= lx || l >= r) return Info(); 
        if (lx >= l && rx <= r) return info[x];
        int m = (lx + rx) / 2;
        push(x);
        return rangeQuery(l, r, 2*x, lx, m) + rangeQuery(l, r, 2*x+1, m, rx);
    }
    
    Info rangeQuery(int l, int r) {
        return rangeQuery(l, r, 1, 0, n);
    }
};
