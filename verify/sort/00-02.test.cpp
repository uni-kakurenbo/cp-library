/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc242/tasks/abc242_b"

#include <iostream>
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "adapter/string.hpp"
#include "iterable/compressed.hpp"
#include "data_structure/segment_tree.hpp"
#include "data_structure/adapter/set.hpp"

signed main() {
    lib::string s; std::cin >> s;
    ITRR(c, s) c -= 'a';
    lib::multiset_adapter<lib::segment_tree> st(ALL(s));
    lib::string res;
    REP(i, s.size()) res += st.kth_smallest(i).value() + 'a';
    print(res);
}
