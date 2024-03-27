/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/vector.hpp"
#include "adaptor/map.hpp"
#include "adaptor/io.hpp"
#include "view/zip.hpp"
#include "data_structure/persistent_dynamic_segment_tree.hpp"
#include "action/range_sum.hpp"
#include "iterable/operation.hpp"

signed main() {
    int n, q; input >> n >> q;
    lib::vector<lib::i32> x(n), y(n), w(n), inds(n);

    input >> lib::views::zip(x, y, w);
    std::iota(ALL(inds), 0);

    debug(inds, x, y, w);

    inds.sort([&](int i, int j) { return x[i] < x[j]; });
    x = lib::order_by(x, inds), y = lib::order_by(y, inds), w = lib::order_by(w, inds);
    debug(inds, x, y, w);

    using segtree = lib::persistent_dynamic_segment_tree<lib::algebraic::addition<lib::i64>>;

    std::vector<segtree> storage;
    segtree data(1'000'000'000 + 1);

    storage.push_back(data);
    ITR(y, w, lib::views::zip(y, w)) {
        storage.push_back(data.add(y, w));
        debug(data);
    }
    debug(storage);

    REP(q) {
        int l, d, r, u; input >> l >> d >> r >> u;
        auto nl = std::ranges::lower_bound(x, l) - std::ranges::begin(x);
        auto nr = std::ranges::lower_bound(x, r) - std::ranges::begin(x);
        debug(l, d, r, u);
        debug(nl, nr);
        auto ans = storage[nr](d, u).fold() + -storage[nl](d, u).fold();
        print(ans);
    }
}
