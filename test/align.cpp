#include "template/standard.hpp"

signed main() {
    int n; cin >> n;
    valarray<i64> a(n); input >> a;
    string s; cin >> s;

    int m; cin >> m;
    debug(lib::ljust(a, m));
    debug(lib::cjust(a, m));
    debug(lib::rjust(a, m));

    debug(lib::ljust(s, m, ' '));
    debug(lib::cjust(s, m, ' '));
    debug(lib::rjust(s, m, ' '));
}
