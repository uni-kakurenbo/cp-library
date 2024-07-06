#pragma once

#include <type_traits>
#include <concepts>


#include "algebraic/null.hpp"
#include "algebraic/helper.hpp"
#include "action/base.hpp"
#include "action/null.hpp"

namespace uni {

namespace actions {


template<class S, auto op, auto e, class F, auto _mapping, auto composition, auto id, auto _power = nullptr>
struct helper {
    static_assert(std::same_as<std::invoke_result_t<decltype(_mapping), F, S>, S>);
    // static_assert(std::same_as<std::invoke_result_t<decltype(_power), F, uni::internal::size_t>, F>);

    using operand = algebraic::monoid_helper<S, op, e>;
    using operation = algebraic::monoid_helper<F, composition, id>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return _mapping(f.val(), x.val());
    }

    static operation power(const operation& x, [[maybe_unused]] const uni::internal::size_t length) noexcept(NO_EXCEPT) {
        if constexpr(_power == nullptr) return x;
        else return _power(x.val(), length);
    }
};


template<class S, class F, auto _mapping, auto _power = nullptr>
struct mixer {
    static_assert(std::same_as<std::invoke_result_t<decltype(_mapping), F, S>, S>);
    static_assert(std::same_as<std::invoke_result_t<decltype(_power), F, uni::internal::size_t>, F>);

    using operand = S;
    using operation = F;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return _mapping(f.val(), x.val());
    }
    static operation power(const operation& x, [[maybe_unused]] const uni::internal::size_t length) noexcept(NO_EXCEPT) {
        if constexpr(_power == nullptr) return x;
        return _power(x.val(), length);
    }
};



template<class S>
struct amplifier {
    using operand = S;
    using operation = S;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f + x;
    }

    static operation power(const operation& x, [[maybe_unused]] const uni::internal::size_t length) noexcept(NO_EXCEPT) {
        return length * x;
    }
};


template<algebraic::internal::magma Magma>
struct make_operatable {
    struct type {
        using operand = Magma;
    };

    static_assert(internal::operatable_action<type>);
};

template<class T>
using make_operatable_t = typename make_operatable<T>::type;


template<algebraic::internal::magma Magma>
struct make_effective {
    struct type : base<Magma> {
        using operation = Magma;
    };

    static_assert(internal::effective_action<type>);
};

template<class T>
using make_effective_t = typename make_effective<T>::type;


template<class T>
struct make_full {
    using type = null<T>;

    static_assert(internal::full_action<type>);
};

template<algebraic::internal::magma Magma>
struct make_full<Magma> {
    using type = make_full<make_operatable_t<Magma>>::type;
};

template<internal::full_action Action>
struct make_full<Action> {
    using type = Action;
};

template<internal::operand_only_action Action>
struct make_full<Action> {
    using base = Action;
    struct type : actions::base<algebraic::null<typename base::operand::value_type>> {
        using operand = typename base::operand;
        using operation = algebraic::null<typename base::operand::value_type>;

        using actions::base<algebraic::null<typename base::operand::value_type>>::base;

        static operand mapping(const operation&, const operand& x) noexcept(NO_EXCEPT) { return x; }
    };

    static_assert(internal::full_action<type>);
};

template<internal::effect_only_action Action>
struct make_full<Action> {
    using base = Action;

    struct type : base {
        using operand = typename base::operation;
        using operation = typename base::operation;

        using base::base;

        static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) { return f + x; }
    };

    static_assert(internal::full_action<type>);
};


template<class T>
using make_full_t = typename make_full<T>::type;


} // namespace actions

} // namespace uni
