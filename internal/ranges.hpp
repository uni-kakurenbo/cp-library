#pragma once

#include "internal/dev_env.hpp"

#if CPP20

#include <ranges>
#include <concepts>
#include <tuple>


namespace lib {

namespace internal {


template<class range>
concept simple_view
  = std::ranges::view<range> && std::ranges::range<const range> &&
    std::same_as<std::ranges::iterator_t<range>, std::ranges::iterator_t<const range>> &&
    std::same_as<std::ranges::sentinel_t<range>, std::ranges::sentinel_t<const range>>;


template<class... Ranges>
concept zip_is_common = (sizeof...(Ranges) == 1 && (std::ranges::common_range<Ranges> && ...))
    || (!(std::ranges::bidirectional_range<Ranges> && ...) && (std::ranges::common_range<Ranges> && ...))
    || ((std::ranges::random_access_range<Ranges> && ...) && (std::ranges::sized_range<Ranges> && ...));


template<bool Const, class... Views>
concept all_random_access = (std::ranges::random_access_range<maybe_const_t<Const, Views>> && ...);

template<bool Const, class... Views>
concept all_bidirectional = (std::ranges::bidirectional_range<maybe_const_t<Const, Views>> && ...);

template<bool Const, class... Views>
concept all_forward = (std::ranges::forward_range<maybe_const_t<Const, Views>> && ...);



template<bool Const, class... Views> struct zip_view_iterator_category {};

template<bool Const, class... Views>
    requires all_forward<Const, Views...>
struct zip_view_iterator_category<Const, Views...> {
    using iterator_category = std::input_iterator_tag;
};


template<bool Const, class... Views>
static auto _most_primitive_iterator_concept() noexcept(NO_EXCEPT) {
    if constexpr(all_random_access<Const, Views...>)
        return std::random_access_iterator_tag{};
    else if constexpr(all_bidirectional<Const, Views...>)
        return std::bidirectional_iterator_tag{};
    else if constexpr(all_forward<Const, Views...>)
        return std::forward_iterator_tag{};
    else
        return std::input_iterator_tag{};
}

template<bool Const, class... Views>
using most_primitive_iterator_concept = decltype(_most_primitive_iterator_concept<Const, Views...>());


template<class Range, bool Const>
using range_iterator_category = typename std::iterator_traits<
        std::ranges::iterator_t<maybe_const_t<Const, Range>>
    >::iterator_category;



template<class Range>
inline static constexpr auto _iterator_concept() noexcept(NO_EXCEPT) {
    if constexpr(std::ranges::random_access_range<Range>)
        return std::random_access_iterator_tag{};
    else if constexpr(std::ranges::bidirectional_range<Range>)
        return std::bidirectional_iterator_tag{};
    else if constexpr(std::ranges::forward_range<Range>)
        return std::forward_iterator_tag{};
    else
        return std::input_iterator_tag{};
}

template<class Range>
using iterator_concept = decltype(_iterator_concept<Range>());


} // namespace internal


namespace views::adapter {


template<class Adapter, class... Args>
concept adapter_invocable = requires { std::declval<Adapter>()(std::declval<Args>()...); };

template<class Adapter, class... Args>
concept adapter_partial_app_viable =
    (Adapter::arity > 1) && (sizeof...(Args) == Adapter::arity - 1) &&
    (std::constructible_from<std::decay_t<Args>, Args> && ...);

template<class Adapter, class... Args> struct partial;

template<class, class> struct pipe;

struct range_adapter_closure {
    template<class Self, typename Range>
        requires std::derived_from<std::remove_cvref_t<Self>, range_adapter_closure> && adapter_invocable<Self, Range>
    friend inline constexpr auto operator|(Range &&range, Self &&self) noexcept(NO_EXCEPT) {
        return std::forward<Self>(self)(std::forward<Range>(range));
    }

    template<class Lhs, typename Rhs>
        requires std::derived_from<Lhs, range_adapter_closure> && std::derived_from<Rhs, range_adapter_closure>
    friend inline constexpr auto operator|(Lhs __lhs, Rhs __rhs) noexcept(NO_EXCEPT) {
        return pipe<Lhs, Rhs>{ std::move(__lhs), std::move(__rhs) };
    }
};

template<class Derived> struct range_adapter {
    template<class... Args>
        requires adapter_partial_app_viable<Derived, Args...>
    inline constexpr auto operator()(Args &&..._args) const noexcept(NO_EXCEPT) {
        return partial<Derived, std::decay_t<Args>...>{
            std::forward<Args>(_args)...};
    }
};

template<class Adapter>
concept closure_has_simple_call_op = Adapter::has_simple_call_op;

template<class Adapter, class... Args>
concept adapter_has_simple_extra_args =
    Adapter::has_simple_extra_args ||
    Adapter::template has_simple_extra_args<Args...>;

template<class Adapter, class... Args>
struct partial : range_adapter_closure {
    std::tuple<Args...> args;

    constexpr partial(Args... _args) noexcept(NO_EXCEPT) : args(std::move(_args)...) {}

    template<class Range>
        requires adapter_invocable<Adapter, Range, const Args &...>
    inline constexpr auto operator()(Range &&range) const & noexcept(NO_EXCEPT) {
        const auto forwarder = [&range](const auto &..._args) constexpr noexcept(NO_EXCEPT) {
            return Adapter{}(std::forward<Range>(range), _args...);
        };
        return std::apply(forwarder, this->args);
    }

    template<class Range>
        requires adapter_invocable<Adapter, Range, Args...>
    inline constexpr auto operator()(Range &&range) && noexcept(NO_EXCEPT) {
        const auto forwarder = [&range](auto &..._args) constexpr noexcept(NO_EXCEPT) {
            return Adapter{}(std::forward<Range>(range), std::move(_args)...);
        };
        return std::apply(forwarder, this->args);
    }

    template<class Range>
    inline constexpr auto operator()(Range &&range) const && = delete;
};

template<class Adapter, typename Arg>
struct partial<Adapter, Arg> : range_adapter_closure {
    Arg arg;

    constexpr partial(Arg _arg) noexcept(NO_EXCEPT) : arg(std::move(_arg)) {}

    template<class Range>
        requires adapter_invocable<Adapter, Range, const Arg &>
    inline constexpr auto operator()(Range &&range) const & noexcept(NO_EXCEPT) {
        return Adapter{}(std::forward<Range>(range), this->arg);
    }

    template<class Range>
        requires adapter_invocable<Adapter, Range, Arg>
    inline constexpr auto operator()(Range &&range) && noexcept(NO_EXCEPT) {
        return Adapter{}(std::forward<Range>(range), std::move(this->arg));
    }

    template<class Range>
    inline constexpr auto operator()(Range &&range) const && = delete;
};

template<class Adapter, class... Args>
    requires adapter_has_simple_extra_args<Adapter, Args...> && (std::is_trivially_copyable_v<Args> && ...)
struct partial<Adapter, Args...> : range_adapter_closure {
    std::tuple<Args...> args;

    constexpr partial(Args... _args) noexcept(NO_EXCEPT) : args(std::move(_args)...) {}

    template<class Range>
        requires adapter_invocable<Adapter, Range, const Args &...>
    inline constexpr auto operator()(Range &&range) const noexcept(NO_EXCEPT) {
        const auto forwarder = [&range](const auto &..._args) constexpr noexcept(NO_EXCEPT) {
            return Adapter{}(std::forward<Range>(range), _args...);
        };
        return std::apply(forwarder, this->args);
    }

    inline static constexpr bool has_simple_call_op = true;
};

template<class Adapter, typename Arg>
    requires adapter_has_simple_extra_args<Adapter, Arg> &&
             std::is_trivially_copyable_v<Arg>
struct partial<Adapter, Arg> : range_adapter_closure {
    Arg arg;

    constexpr partial(Arg _arg) noexcept(NO_EXCEPT) : arg(std::move(_arg)) {}

    template<class Range>
        requires adapter_invocable<Adapter, Range, const Arg &>
    inline constexpr auto operator()(Range &&range) const noexcept(NO_EXCEPT) {
        return Adapter{}(std::forward<Range>(range), this->arg);
    }

    inline static constexpr bool has_simple_call_op = true;
};

template<class Lhs, typename Rhs, typename Range>
concept pipe_invocable = requires {
    std::declval<Rhs>()(std::declval<Lhs>()(std::declval<Range>()));
};

template<class Lhs, typename Rhs> struct pipe : range_adapter_closure {
    [[no_unique_address]] Lhs lhs;
    [[no_unique_address]] Rhs rhs;

    constexpr pipe(Lhs __lhs, Rhs __rhs) noexcept(NO_EXCEPT) : lhs(std::move(__lhs)), rhs(std::move(__rhs)) {}

    template<class Range>
        requires pipe_invocable<const Lhs &, const Rhs &, Range>
    inline constexpr auto operator()(Range &&range) const & noexcept(NO_EXCEPT) {
        return rhs(lhs(std::forward<Range>(range)));
    }

    template<class Range>
        requires pipe_invocable<Lhs, Rhs, Range>
    inline constexpr auto operator()(Range &&range) && noexcept(NO_EXCEPT) {
        return std::move(rhs)(std::move(lhs)(std::forward<Range>(range)));
    }

    template<class Range>
    inline constexpr auto operator()(Range &&range) const && = delete;
};

template<class Lhs, typename Rhs>
    requires closure_has_simple_call_op<Lhs> && closure_has_simple_call_op<Rhs>
struct pipe<Lhs, Rhs> : range_adapter_closure {
    [[no_unique_address]] Lhs lhs;
    [[no_unique_address]] Rhs rhs;

    constexpr pipe(Lhs __lhs, Rhs __rhs) noexcept(NO_EXCEPT) : lhs(std::move(__lhs)), rhs(std::move(__rhs)) {}

    template<class Range>
        requires pipe_invocable<const Lhs &, const Rhs &, Range>
    inline constexpr auto operator()(Range &&range) const noexcept(NO_EXCEPT) {
        return rhs(lhs(std::forward<Range>(range)));
    }

    inline static constexpr bool has_simple_call_op = true;
};


} // namespace views::adapter


} // namespace lib


#endif
