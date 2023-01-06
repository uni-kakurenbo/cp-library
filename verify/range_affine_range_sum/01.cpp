#include <atcoder/modint>
#include "template.hpp"

#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_affine_range_sum.hpp"

#include <vector>

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<atcoder::modint998244353> a(n); input >> a;

    lib::implicit_treap<lib::actions::range_affine_range_sum<atcoder::modint998244353>> data(ALL(a));
    debug(data);

    LOOP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, b, c; std::cin >> l >> r >> b >> c;
            data.apply(l, r, { b, c });
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r;
            print(data.prod(l, r));
        }
        debug(data);
    }
}
