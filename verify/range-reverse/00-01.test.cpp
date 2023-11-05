/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://atcoder.jp/contests/arc153/tasks/arc153_b"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "grid.hpp"
#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/helpers.hpp"

signed main() {
    int h, w; std::cin >> h >> w;
    lib::grid<char,lib::string> G(h, w); input >> G;

    lib::implicit_treap<lib::actions::make_full_t<int>> s, t;

    REP(i, h) s.push_back(i);
    REP(i, w) t.push_back(i);

    int q; std::cin >> q;
    REP(q) {
        int a, b; std::cin >> a >> b;
        s.reverse(0, a), s.reverse(a, h);
        t.reverse(0, b), t.reverse(b, w);
    }

    REP(i, h) {
        REP(j, w) print << G(s[i].val().val(), t[j].val().val());
        print();
    }

    return 0;
}
