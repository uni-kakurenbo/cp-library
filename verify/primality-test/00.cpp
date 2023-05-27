#include <iostream>
#include "template.hpp"
#include "numeric/prime.hpp"

int main() {
    int q; cin >> q;
    REP(q) {
        i64 n; cin >> n;
        print(lib::is_prime(n) ? "Yes" : "No");
    }
}
