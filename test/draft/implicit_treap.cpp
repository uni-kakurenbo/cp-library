#include "data_structure/monoid/addition.hpp"
#include "data_structure/internal/is_monoid.hpp"
#include "data_structure/monoid/assignment.hpp"

#include "template.hpp"

namespace lib {

namespace actions {


template<class T> struct range_set_range_sum {
    using operand = monoids::sum<T>;
    using operation = monoids::assignment<T>;

    static operand map(const operand& x, const operation& f) {
        return { fold(f, x.len())->value_or(x->val()) };
    }
    static operation fold(const operation& x, const internal::size_t length) {
        if(x->has_value()) return { x->operator*() * length };
        return x;
    }
};


} // namespace actions

} // namespace lib

signed main() {
    debug(lib::internal::is_monoid_v<int>);
    debug(lib::internal::is_monoid_v<lib::monoids::addition<int>>);
    debug(lib::actions::range_set_range_sum::operand::specialized);
    // lib::implicit_treap<lib::actions::range_set_range_sum<int>> data(5, 1);
    // debug(data, data.fold());
    // data.apply(1, 5);
    // data.apply(1, 3, 5);
    // debug(data, data.fold());
}
