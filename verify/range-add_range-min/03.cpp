#include <iostream>

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

// #include "template.hpp"

#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_affine_range_minmax.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::implicit_treap<lib::actions::range_affine_range_minmax<ll>> data(n);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; ++r;
            data(l, r) <<= { 1, x };
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; ++r;
            std::cout << data(l, r).fold().first << "\n";
        }
        // debug(data);
    }
}
