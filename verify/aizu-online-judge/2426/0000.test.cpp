/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2426"

#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "structure/grid.hpp"
#include "view/zip.hpp"
#include "iterable/compressed.hpp"
#include "iterable/accumulation.hpp"

signed main() {
    int n, m; input >> n >> m;
    uni::vector<uni::i64> x(n), y(n); input >> uni::views::zip(x, y);

    uni::compressed comp_x(x), comp_y(y);
    debug(comp_x, comp_y);

    uni::grid<uni::i64> grid(comp_x.rank_sup(), comp_y.rank_sup());

    ITR(x, y, uni::views::zip(comp_x, comp_y)) grid(x, y) += 1;

    uni::accumulation_2d cum(grid);
    REP(m) {
        uni::i64 x0, y0, x1, y1; input >> x0 >> y0 >> x1 >> y1;
        debug(x0, y0, x1, y1);
        print(cum({ comp_x.rank(x0), comp_x.rank2(x1) + 1 }, { comp_y.rank(y0), comp_y.rank2(y1) + 1 }));
    }
}
