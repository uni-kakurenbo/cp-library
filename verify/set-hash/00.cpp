// #include <bits/stdc++.h>
#include "template.hpp"
#include "hash/set_hasher.hpp"


signed main() {
    int n; std::cin >> n;

    lib::set_hasher<int> hash_a, hash_b;
    std::vector<lib::set_hasher<int>::hash_type> a(n), b(n);

    REP(i, n) {
        int v; std::cin >> v; hash_a.insert(v);
        a[i] = hash_a();
    }
    REP(i, n) {
        int v; std::cin >> v; hash_b.insert(v);
        b[i] = hash_b();
    }

    debug(a, b);

    int q; std::cin >> q;
    REP(q) {
        int p, q; std::cin >> p >> q; --p, --q;
        print((a[p] == b[q] ? "Yes" : "No"));
    }

    return 0;
}
