#include <bits/stdc++.h>
#include "template.hpp"
#include "numeric/fast_prime.hpp"

signed main() {
    int n; std::cin >> n;
    print << n << ": ";
    ITR(p, v, lib::factorize(n)) {
        REP(v) print << p << " ";
    }
    print.seekp(-1);
    print();
}
