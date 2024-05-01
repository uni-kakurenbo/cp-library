/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0342"
#define ERROR 1e-8

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "geometry/point.hpp"
#include "geometry/polygon.hpp"
#include "geometry/convex_hull.hpp"
#include "numeric/float.hpp"
#include "adaptor/io.hpp"
#include "adaptor/array.hpp"
#include "view/zip.hpp"
#include "view/cyclic.hpp"
#include "data_structure/disjoint_set_union.hpp"

using point = uni::point<uni::ld>;

signed main() {
    uni::i32 v, r; input >> v >> r;
    uni::polygon<point> ps(v); input >> ps;
    uni::valarray<uni::i32> s(r), t(r); input >> uni::views::zip(s, t);
    s -= 1, t -= 1;

    uni::dsu ds(v);

    auto ch = ps.convex_hull();
    debug(ch);

    const auto n = ch.ssize();

    uni::vector<uni::i32> chv;
    REP(i, n) {
        REP(j, v) {
            if(uni::compare(ch[i], ps[j]) == 0) {
                chv.push_back(j);
                break;
            }
        }
    }

    auto view = chv | uni::views::cyclic;
    debug(chv);

    uni::ld ans = 0;

    REP(i, n) {
        auto a = view[i];
        auto b = view[i + 1];
        ans += std::abs(ps[a] - ps[b]);
        ds.merge(a, b);
    }

    uni::vector<std::pair<uni::ld, uni::i32>> edges;
    REP(i, r) {
        edges.emplace_back(std::abs(ps[s[i]] - ps[t[i]]), i);
    }
    edges.sort();

    ITR(cost, p, edges) {
        const auto a = s[p];
        const auto b = t[p];

        if(!ds.same(a, b)) {
            ans += cost;
            ds.merge(a, b);
        }
    }

    print(ans);
}
