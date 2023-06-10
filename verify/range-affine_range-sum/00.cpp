#include "template.hpp"

#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/range_affine_range_sum.hpp"

#include <vector>

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<lib::modint998244353> a(n); input >> a;

    lib::lazy_segment_tree<lib::actions::range_affine_range_sum<lib::modint998244353>> data(ALL(a));
    debug(data);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, b, c; std::cin >> l >> r >> b >> c;
            data(l, r).affine(b, c);
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r;
            print(data(l, r).sum());
        }
        debug(data);
    }
}
