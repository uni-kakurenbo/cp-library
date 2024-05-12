/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"

#include <iostream>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "numeric/matrix.hpp"
#include "numeric/modular/modint.hpp"

using mint = uni::modint998244353;

signed main() {
    uni::i32 n, m; input >> n >> m;
    uni::matrix<mint> a(n, m); input >> a;
    uni::vector<mint> b(n); input >> b;
    debug(a, b);

    uni::matrix<mint> res;
    auto rank = a.solve_linear_equation(b, &res);

    if(rank.has_value()) {
        print(m - *rank);
        print(res);
    }
    else {
        print(-1);
    }
}
