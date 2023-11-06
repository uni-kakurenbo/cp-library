#include <iostream>
#include <vector>

#include "template/standard.hpp"
#include "iterable/compressed.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    compressed comp_a(ALL(a));
    debug(comp_a);
    REP(i, n) debug(comp_a(comp_a[i]));
    return 0;
}
