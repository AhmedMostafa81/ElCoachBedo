
enum class BitOp { OR, AND, XOR };

template <typename T>
void fwht(std::vector<T>& a, BitOp op, bool inv = false) {
    int n = a.size();
    // TODO: USE MOD HERE IF NEEDED
    for (int s = 2, h = 1; s <= n; s <<= 1, h <<= 1) {
        for (int l = 0; l < n; l += s) {
            for (int i = 0; i < h; i++) {
                T u = a[l + i];
                T v = a[l + h + i];
                
                if (op == BitOp::OR) {
                    a[l + h + i] = inv ? (v - u) : (v + u);
                } else if (op == BitOp::AND) {
                    a[l + i] = inv ? (u - v) : (u + v);
                } else if (op == BitOp::XOR) {
                    a[l + i] = u + v;
                    a[l + h + i] = u - v;
                }
            }
        }
    }
    // Only XOR requires scaling down by N during the inverse transform
    // TODO: USE MODULO INVERSE IF NEEDED
    if (op == BitOp::XOR && inv) {
        for (int i = 0; i < n; i++) a[i] /= n;
    }
}

template <typename T>
std::vector<T> convolution(std::vector<T> a, std::vector<T> b, BitOp op) {
    // 1. Find the next power of 2 that fits both arrays
    int n = std::max(a.size(), b.size());
    int sz = 1;
    while (sz < n) sz <<= 1;
    
    // 2. Pad with zeros
    a.resize(sz, 0);
    b.resize(sz, 0);

    // 3. Forward transform both
    fwht(a, op);
    fwht(b, op);

    // 4. Point-wise multiplication
    // TODO: USE MOD HERE IF NEEDED
    for (int i = 0; i < sz; i++) {
        a[i] *= b[i];
    }

    // 5. Inverse transform to get the final array
    fwht(a, op, true);
    return a;
}
