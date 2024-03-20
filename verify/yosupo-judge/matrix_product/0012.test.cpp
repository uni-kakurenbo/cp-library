/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "numeric/modular/modint.hpp"
#include "numeric/matrix.hpp"

signed main() {
    lib::dynamic_builtin_modint_64bit<-1>::set_mod(998244353);

    int n, m, k; std::cin >> n >> m >> k;
    lib::matrix<lib::dynamic_builtin_modint_64bit<-1>> a(n, m), b(m, k);
    input >> a >> b;
    print(a * b);
}
