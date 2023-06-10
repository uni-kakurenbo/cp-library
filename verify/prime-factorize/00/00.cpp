// #include <bits/stdc++.h>
#include "template.hpp"
#include "numeric/fast_prime.hpp"

signed main() {
    int q; std::cin >> q;
    REP(q) {
        ll a; std::cin >> a;
        auto facts = lib::factorize(a);
        print(facts.size(), facts);
    }
}
