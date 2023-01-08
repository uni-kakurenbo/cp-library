#include <bits/stdc++.h>
#include "template.hpp"
#include "numeric/prime.hpp"

signed main() {
    int q; std::cin >> q;
    REP(q) {
        ll a; std::cin >> a;
        auto facts = lib::prime_factors(a);
        print(facts.size(), facts);
    }
}
