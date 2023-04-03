#include <bits/stdc++.h>
using namespace std;

#include "numeric/math.hpp"
#include "template.hpp"

using mint = lib::modint998244353;

signed main() {
    i64 n, r; std::cin >> n >> r;
    print(lib::nPr(n, r), lib::nCr(n, r));
    lib::small_binomial<i64> binom0(n);
    print(binom0.nPr(n, r), binom0.nCr(n, r));
    lib::small_binomial<mint> binom1(n);
    print(binom1.nPr(n, r), binom1.nCr(n, r));
    lib::small_binomial_prime_mod<mint> binom2(n);
    print(binom2.nPr(n, r), binom2.nCr(n, r));
    lib::large_binomial_prime_mod<mint> binom3(n);
    print(binom3.nPr(n, r), binom3.nCr(n, r));
    lib::large_binomial_fixed_power_prime_mod<mint> binom4(n);
    print(binom4.nPr(r), binom4.nCr(r));
}
