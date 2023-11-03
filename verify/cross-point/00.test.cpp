#define PROBLEM "https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_ah"
#define IGNORE

#include "template.hpp"

using ld = long double;

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        solve();
    }
    return 0;
}

void solve() {
    lib::point<ld> p0, p1, p2, p3;
    input >> p0 >> p1 >> p2 >> p3;

    lib::segment s0(p0, p1), s1(p2, p3);

    print.conditional(lib::is_intersecting(s0, s1), "Yes", "No");

    // auto p = lib::intersection(lib::line(s0), lib::line(s1));
    // if(p.has_value()) {
    //     debug(static_cast<i64>(lib::relation(*p, lib::line(s0))));
    //     debug(static_cast<i64>(lib::relation(*p, lib::line(s1))));
    //     debug(static_cast<i64>(lib::relation(*p, s0)));
    //     debug(static_cast<i64>(lib::relation(*p, s1)));
    // }
}
