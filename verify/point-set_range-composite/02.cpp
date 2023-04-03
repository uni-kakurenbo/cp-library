#include "template.hpp"

#include "data_structure/implicit_treap.hpp"
#include "data_structure/monoid/affine.hpp"
#include "data_structure/monoid/assignment.hpp"

#include "data_structure/range_action/flags.hpp"

#include <vector>

using lib::monoids::affine;
using mint = atcoder::modint998244353;

struct action {
    static constexpr lib::actions::flags tags{};

    using operand = affine<mint,true>;
    using operation = lib::monoids::assignment<std::pair<mint,mint>>;

    static operand map(const operand& x, const operation& f) { return f->value_or(std::pair<mint,mint>{x->first, x->second}); }
    static operation fold(const operation& x, const lib::internal::size_t) { return x; }
};


signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<std::pair<int,int>> f(n); input >> f;
    debug(f);

    lib::implicit_treap<action> data(ALL(f));
    debug(data);

    LOOP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, a, b; std::cin >> p >> a >> b;
            data.apply(p, std::pair<int,int>{ a, b });
        }
        if(t == 1) {
            int l, r, x; std::cin >> l >> r >> x;
            auto [a, b] = data.fold(l, r);
            print(a * x + b);
        }
        debug(data);
    }
}
