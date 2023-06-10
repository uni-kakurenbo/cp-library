// #include <bits/stdc++.h>
#include <iostream>

// #include "template.hpp"

#include "snippet/iterations.hpp"
#include "snippet/aliases.hpp"

#include "data_structure/segment_tree.hpp"
#include "data_structure/range_action/range_min.hpp"



signed main() {
    int n, q; std::cin >> n >> q;
    lib::segment_tree<lib::actions::range_min<int>> data(n);
    // debug(data);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, x; std::cin >> p >> x;;
            data[p] = x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; ++r;
            std::cout << data(l, r).min() << "\n";
        }
        // debug(data);
    }

    return 0;
}
