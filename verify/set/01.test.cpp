#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/segment_tree.hpp"
#include "data_structure/adapter/set.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    valarray<bool> t(n);

    REP(i, n) {
        char v; cin >> v;
        t[i] = v == '1';
    }

    lib::set_adapter<lib::segment_tree> st(n);
    st.build_from_bits(ALL(t));
    debug(st);

    REP(q) {
        debug(st);
        int t; cin >> t;
        if(t == 0) {
            int k; cin >> k;
            st.insert(k);
        }
        if(t == 1) {
            int k; cin >> k;
            st.remove(k);
        }
        if(t == 2) {
            int k; cin >> k;
            print(st.contains(k));
        }
        if(t == 3) {
            int k; cin >> k;
            print(st.next(k).value_or(-1));
        }
        if(t == 4) {
            int k; cin >> k;
            print(st.prev(k).value_or(-1));
        }
    }
}
