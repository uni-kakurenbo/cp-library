/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://atcoder.jp/contests/abc250/tasks/abc250_e"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/vector.hpp"
#include "hash/set_hasher.hpp"

signed main() {
    int n; std::cin >> n;

    lib::set_hasher<int> hash_a, hash_b;
    lib::vector<lib::set_hasher<int>::hash_type> a(n), b(n);

    REP(i, n) {
        int v; std::cin >> v; hash_a.insert(v);
        a[i] = hash_a();
    }
    REP(i, n) {
        int v; std::cin >> v; hash_b.insert(v);
        b[i] = hash_b();
    }

    int q; std::cin >> q;
    REP(q) {
        int p, q; std::cin >> p >> q; --p, --q;
        print.yesno(a[p] == b[q]);
    }

    return 0;
}
