#include <iostream>
#include <vector>

#include "template/standard.hpp"
#include "iterable/inverse.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];

    inverse inv(ALL(a));
    debug(inv);

    return 0;
}
