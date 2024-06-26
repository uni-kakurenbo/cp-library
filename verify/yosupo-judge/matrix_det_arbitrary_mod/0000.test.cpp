/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "numeric/modular/modint.hpp"
#include "numeric/matrix.hpp"

using mint = uni::dynamic_barrett_modint_32bit<-1>;

signed main() {
    uni::i32 n, m; input >> n >> m;
    mint::set_mod(m);
    uni::matrix<mint> a(n, n); input >> a;
    print(a.determinant());
}
