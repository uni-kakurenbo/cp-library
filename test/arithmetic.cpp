#include "template/standard.hpp"

signed main() {
    i64 n, p; std::cin >> n >> p;
    REP(i, n) {
        auto v = lib::to_base_n_vector(i, p);
        debug(i, v);
        debug(lib::from_base_n_sequence(v, p));
    }
}
