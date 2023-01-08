#include <iostream>

#include "snippet/aliases.hpp"

#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/range_add_range_sum.hpp"
#include "data_structure/monoid/addition.hpp"

// #include "template.hpp"


signed main() {
    int n, q; std::cin >> n >> q;
    lib::lazy_segment_tree<lib::actions::range_add_range_sum<ll>> data(n);
    // debug(data, data._values, data._lengths);

    for(;q--;) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; --l;
            data.add(l, r, x);
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; --l;
            std::cout << data.sum(l, r) << "\n";
        }
        // debug(data);
    }
}
