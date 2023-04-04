#include "template.hpp"

#include "data_structure/segment_tree.hpp"
#include "algebraic/affine.hpp"

#include <vector>

using lib::algebraic::affine;
using mint = atcoder::modint998244353;

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<std::pair<int,int>> f(n); input >> f;
    debug(f);

    lib::segment_tree<affine<mint,true>> data(ALL(f));
    debug(data);

    LOOP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, a, b; std::cin >> p >> a >> b;
            data.set(p, { a, b });
        }
        if(t == 1) {
            int l, r, x; std::cin >> l >> r >> x;
            auto [a, b] = data.fold(l, r).val();
            print(a * x + b);
        }
        debug(data);
    }
}
