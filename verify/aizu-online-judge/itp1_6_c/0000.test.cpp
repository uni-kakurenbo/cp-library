/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_6_C"

#include <ranges>

#include "adaptor/io.hpp"
#include "adaptor/multi_container.hpp"
#include "view/repeat.hpp"
#include "iterable/operation.hpp"

signed main() {
    uni::multi_container<int, 3> cnt(4, 3, 10);

    int n; input >> n;

    REP(i, n) {
        int b, f, r, v; input >> b >> f >> r >> v; --b, --f, --r;
        cnt[b][f][r] += v;
    }

    REP(i, 4) {
        ITR(v, cnt[i]) print << " ", print(v);
        if(i < 3) print(uni::join(uni::views::repeat('#') | std::views::take(20)));
    }
}
