#include "template.hpp"

#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_affine_range_sum.hpp"

#include <vector>

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<modint998244353> a(n); input >> a;

    lib::implicit_treap<lib::actions::range_affine_range_sum<modint998244353>> data(ALL(a));
    debug(data);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, x; std::cin >> p >> x;
            data.insert(p, x);
        }
        if(t == 1) {
            int p; std::cin >> p;
            data.erase(p);
        }
        if(t == 2) {
            int l, r; std::cin >> l >> r;
            data.reverse(l, r);
        }
        if(t == 3) {
            int l, r, b, c; std::cin >> l >> r >> b >> c;
            data(l, r) <<= { b, c };
        }
        if(t == 4) {
            int l, r; std::cin >> l >> r;
            print(data(l, r).fold());
        }
        debug(data);
    }
}
