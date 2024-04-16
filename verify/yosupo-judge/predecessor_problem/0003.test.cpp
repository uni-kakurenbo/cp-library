/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/valarray.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/dynamic_set.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    uni::dynamic_set<uni::i32, uni::treap_context<uni::i32>> st;

    REP(i, n) {
        char v; std::cin >> v;
        if(v == '1') st.insert(i);
    }

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int k; std::cin >> k;
            st.insert<true>(k);
        }
        if(t == 1) {
            int k; std::cin >> k;
            st.erase(k);
        }
        if(t == 2) {
            int k; std::cin >> k;
            print(st.contains(k));
        }
        if(t == 3) {
            int k; std::cin >> k;
            auto itr = st.lower_bound(k);
            print(itr == st.end() ? -1 : *itr);
        }
        if(t == 4) {
            int k; std::cin >> k;
            auto itr = st.upper_bound(k);
            print(itr == st.begin() ? -1 : *std::ranges::prev(itr));
        }
        debug(st);
    }
}
