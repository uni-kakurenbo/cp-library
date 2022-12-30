#include <iostream>
#include "data_structure/range_operation/range_add_range_sum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::range_add_range_sum<ll,ll> data(n);

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
    }
}
