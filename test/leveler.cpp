#include "template.hpp"

#include "numeric/leveler.hpp"

signed main() {
    int n; std::cin >> n;
    int m; cin >> m;
    valarray<i64> p(m); input >> p;

    lib::leveler<int> leveler(p);

    debug(leveler.max(), leveler.dimension());
    REP(i, n) {
        auto v = leveler.revert(i);
        debug(i, v);
        debug(leveler.convert(v));
    }
}
