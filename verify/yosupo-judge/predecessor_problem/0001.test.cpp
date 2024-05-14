/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/valarray.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/adaptor/set.hpp"
#include "data_structure/segment_tree.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::valarray<bool> t(n);

    REP(i, n) {
        char v; input >> v;
        t[i] = v == '1';
    }

    uni::set_adaptor<uni::segment_tree> st(n);
    st.build_from_bits(ALL(t));

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 k; input >> k;
            st.insert(k);
        }
        if(t == 1) {
            uni::i32 k; input >> k;
            st.remove(k);
        }
        if(t == 2) {
            uni::i32 k; input >> k;
            print(st.contains(k));
        }
        if(t == 3) {
            uni::i32 k; input >> k;
            print(st.next(k).value_or(-1));
        }
        if(t == 4) {
            uni::i32 k; input >> k;
            print(st.prev(k).value_or(-1));
        }
    }
}
