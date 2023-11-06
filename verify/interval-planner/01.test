/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc293/tasks/abc293_g"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "adapter/valarray.hpp"
#include "adapter/vector.hpp"
#include "iterable/operation.hpp"
#include "numeric/interval_scanner.hpp"

signed main() {
    int n, q; input >> n >> q;
    lib::vector<lib::i64> a(n); input >> a;

    lib::vector<lib::spair<int>> queries(q); input >> queries;
    ITRR(query, queries) query.first--;

    lib::i64 res = 0;
    std::vector<int> cnt(200'001, 0);

    auto expand = [&](int p) {
        if(cnt[a[p]] >= 2) res += 1LL * (cnt[a[p]]) * (cnt[a[p]] - 1) / 2;
        cnt[a[p]]++;
    };
    auto contract = [&](int p) {
        cnt[a[p]]--;
        if(cnt[a[p]] >= 2) res -= 1LL * (cnt[a[p]]) * (cnt[a[p]] - 1) / 2;
    };
    auto evaludate = [&]() { return res; };

    lib::interval_plannner planner(expand, contract, evaludate);
    print(lib::join(planner.scan(queries), "\n"));
}
