#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"

#include "template.hpp"

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
            print(data(l, r).fold());
        }
        // debug(data);
    }

    return 0;
}
