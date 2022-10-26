#include <iostream>
#include <vector>

#include "template.hpp"
#include "inverse.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];

    Inverse inv(ALL(a));
    debug(inv);

    return 0;
}
