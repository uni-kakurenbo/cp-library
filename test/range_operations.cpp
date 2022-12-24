// https://judge.yosupo.jp/problem/range_affine_range_sum

#include <atcoder/modint>
#include "template.hpp"

#include "data_structure/range_operations.hpp"
using namespace Lib;

#include <vector>

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<atcoder::modint998244353> a(n); input >> a;

    Lib::RangeAffineRangeSum<atcoder::modint998244353> data;
    data.insert(ALL(a));
    debug(data);

    LOOP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, b, c; std::cin >> l >> r >> b >> c;
            data.update(l, r, {b, c});
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r;
            std::cout << data.query(l, r).val() << "\n";
        }
        debug(data);
    }
}
