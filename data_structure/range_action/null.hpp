#pragma once


#include "internal/dev_env.hpp"
#include "internal/exception.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/null.hpp"


namespace lib {

namespace actions {


template<class T> struct null : base<algebraic::null<T>> {
    static constexpr flags tags{ flags::implicit_treap };

    using operand = monoids::addition<T>;
    using operation = algebraic::null<T>;

    static operand map(const operand& x, const operation&) noexcept(NO_EXCEPT) { return x; }
};


} // namespace actions


template<class T> struct implicit_treap<actions::null<T>> : internal::implicit_treap_impl::core<actions::null<T>> {
    using internal::implicit_treap_impl::core<actions::null<T>>::core;

    template<class... Args> void apply(const Args&... args) = delete;
    template<class... Args> void fold(const Args&... args) = delete;
};


} // namespace lib
