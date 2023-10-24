#include <iostream>

#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_add_range_sum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::implicit_treap<lib::actions::range_add_range_sum<long>> data(n);
    // debug(data);

    for(;q--;) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; --l;
            data(l, r) += x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; --l;
            std::cout << data(l, r).fold().val() << "\n";
        }
        // debug(data);
    }
}
