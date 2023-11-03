#define PROBLEM "https://judge.yosupo.jp/problem/range_reverse_range_sum"

#include "template.hpp"
#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_add_range_sum.hpp"

int main() {
    int n, q; cin >> n >> q;
    valarray<i64> a(n); input >> a;
    lib::implicit_treap<lib::actions::range_add_range_sum<i64>> data(ALL(a));
    debug(data);

    REP(q) {
        int t, l, r; cin >> t >> l >> r;
        if(t == 0) {
            data.reverse(l, r);
        }
        if(t == 1) {
            print(data(l, r).fold());
        }
    }
}
