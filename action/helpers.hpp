#pragma once

#include <type_traits>
#include <concepts>

#include "algebraic/null.hpp"
#include "algebraic/helper.hpp"
#include "action/base.hpp"
#include "action/null.hpp"

namespace lib {

namespace actions {


template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)(), F (*folding)(F, int) = nullptr>
struct helper : lib::actions::base<algebraic::monoid_helper<F,composition,id>> {
    using operand = algebraic::monoid_helper<S,op,e>;
    using operation = algebraic::monoid_helper<F,composition,id>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        return mapping(y.val(), x.val());
    }
    static operation fold(const operation& x, [[maybe_unused]] const lib::internal::size_t length) noexcept(NO_EXCEPT) {
        if constexpr(folding == nullptr) return x;
        return folding(x.val(), length);
    }
};


template<class T>
struct make_full {
    using type = null<T>;

    static_assert(internal::full_action<type>);
};

template<class Monoid>
    requires algebraic::internal::monoid<Monoid>
struct make_full<Monoid> {
    using base = Monoid;

    struct type {
        using operand = base;
        using operation = algebraic::null<typename operand::value_type>;
        static operand map(const operand& x, const operation&) noexcept(NO_EXCEPT) { return x; }
        static operation fold(const operation& x, const lib::internal::size_t) noexcept(NO_EXCEPT) { return x; }
    };

    static_assert(internal::full_action<type>);
};

template<class Action>
    requires internal::full_action<Action>
struct make_full<Action> {
    using type = Action;
};

template<class Action>
    requires internal::operand_only_action<Action>
struct make_full<Action> {
    using base = Action;
    using type = typename make_full<typename base::operand>::type;

    static_assert(internal::full_action<type>);
};

template<class Action>
    requires internal::effect_only_action<Action>
struct make_full<Action> {
    using base = Action;

    struct type {
        using operation = typename base::operation;
        using operand = typename algebraic::null<typename operation::value_type>;
        static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) { return base::map(x, y); }
        static operation fold(const operation& x, const lib::internal::size_t len) noexcept(NO_EXCEPT) { return base::fold(x, len); }
    };

    static_assert(internal::full_action<type>);
};


template<class T>
using make_full_t = typename make_full<T>::type;


} // namespace actions

} // namespace lib
