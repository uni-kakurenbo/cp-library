#include "template/standard.hpp"

signed main() {
    i64 n; cin >> n;
    lib::prime_enumerator<i64> primes(n);
    debug(primes);
    debug(primes | std::views::reverse);
    REP(i, n) debug(i, primes.count(i));
}
