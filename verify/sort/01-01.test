/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc294/tasks/abc294_c"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "view/concat.hpp"
#include "iterable/compressed.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/adapter/set.hpp"

signed main() {
    int n, m; std::cin >> n >> m;
    lib::valarray<lib::i64> a(n), b(m); input >> a >> b;
    lib::compressed comp(lib::views::concat(a, b));
    lib::multiset_adapter st(comp);
    auto f = [&](int v) { return st.count_or_under(comp.rank(v)); };
    print(a | std::views::transform(f));
    print(b | std::views::transform(f));
}
