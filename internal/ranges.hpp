#pragma once


#include <ranges>
#include <concepts>
#include <tuple>


#include "internal/dev_env.hpp"
#include "internal/type_traits.hpp"


namespace lib {

namespace internal {


template<class Range>
concept resizable_range
  = std::ranges::range<Range> &&
    requires (Range& r) { r.resize(0); };

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
concept all_contiguous = (std::ranges::contiguous_range<maybe_const_t<Const, Views>> && ...);

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
    if constexpr(all_contiguous<Const, Views...>)
        return std::contiguous_iterator_tag{};
    else if constexpr(all_random_access<Const, Views...>)
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
static constexpr auto _iterator_concept() noexcept(NO_EXCEPT) {
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


namespace views::adaptor {


template<class Adaptor, class... Args>
concept adaptor_invocable = requires { std::declval<Adaptor>()(std::declval<Args>()...); };

template<class Adaptor, class... Args>
concept adaptor_partial_app_viable =
    (Adaptor::arity > 1) && (sizeof...(Args) == Adaptor::arity - 1) &&
    (std::constructible_from<std::decay_t<Args>, Args> && ...);

template<class Adaptor, class... Args> struct partial;

template<class, class> struct pipe;


template <class Derived> struct range_adaptor_closure {};


template <class T, class U>
    requires(!std::same_as<T, range_adaptor_closure<U>>)
void is_range_adaptor_closure_fn(const T &, const range_adaptor_closure<U> &);


template <class T>
concept is_range_adaptor_closure = requires(T t) { adaptor::is_range_adaptor_closure_fn(t, t); };


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdangling-reference"


template <class Self, class Range>
    requires is_range_adaptor_closure<Self> && adaptor_invocable<Self, Range>
constexpr auto operator|(Range&& range, Self&& self) {
    return std::forward<Self>(self)(std::forward<Range>(range));
}


template <class Lhs, class Rhs>
    requires is_range_adaptor_closure<Lhs> && is_range_adaptor_closure<Rhs>
constexpr auto operator|(Lhs&& lhs, Rhs&& rhs) {
    return pipe<std::decay_t<Lhs>, std::decay_t<Rhs>>{ std::forward<Lhs>(lhs), std::forward<Rhs>(rhs)};
}


#pragma GCC diagnostic pop


template<class Derived> struct range_adaptor {
    template<class... Args>
        requires adaptor_partial_app_viable<Derived, Args...>
    inline constexpr auto operator()(Args&& ..._args) const noexcept(NO_EXCEPT) {
        return partial<Derived, std::decay_t<Args>...>{
            std::forward<Args>(_args)...
        };
    }
};

template<class Adaptor>
concept closure_has_simple_call_op = Adaptor::has_simple_call_op;

template<class Adaptor, class... Args>
concept adaptor_has_simple_extra_args =
    Adaptor::has_simple_extra_args ||
    Adaptor::template has_simple_extra_args<Args...>;

template<class Adaptor, class... Args>
struct partial : range_adaptor_closure<partial<Adaptor, Args...>> {
    std::tuple<Args...> args;

    constexpr partial(Args... _args) noexcept(NO_EXCEPT) : args(std::move(_args)...) {}

    template<class Range>
        requires adaptor_invocable<Adaptor, Range, const Args &...>
    inline constexpr auto operator()(Range&& range) const & noexcept(NO_EXCEPT) {
        const auto forwarder = [&range](const auto &..._args) constexpr noexcept(NO_EXCEPT) {
            return Adaptor{}(std::forward<Range>(range), _args...);
        };
        return std::apply(forwarder, this->args);
    }

    template<class Range>
        requires adaptor_invocable<Adaptor, Range, Args...>
    inline constexpr auto operator()(Range&& range) && noexcept(NO_EXCEPT) {
        const auto forwarder = [&range](auto &..._args) constexpr noexcept(NO_EXCEPT) {
            return Adaptor{}(std::forward<Range>(range), std::move(_args)...);
        };
        return std::apply(forwarder, this->args);
    }

    template<class Range>
    inline constexpr auto operator()(Range&& range) const && = delete;
};

template<class Adaptor, class Arg>
struct partial<Adaptor, Arg> : range_adaptor_closure<partial<Adaptor, Arg>> {
    Arg arg;

    constexpr partial(Arg _arg) noexcept(NO_EXCEPT) : arg(std::move(_arg)) {}

    template<class Range>
        requires adaptor_invocable<Adaptor, Range, const Arg &>
    inline constexpr auto operator()(Range&& range) const & noexcept(NO_EXCEPT) {
        return Adaptor{}(std::forward<Range>(range), this->arg);
    }

    template<class Range>
        requires adaptor_invocable<Adaptor, Range, Arg>
    inline constexpr auto operator()(Range&& range) && noexcept(NO_EXCEPT) {
        return Adaptor{}(std::forward<Range>(range), std::move(this->arg));
    }

    template<class Range>
    inline constexpr auto operator()(Range&& range) const && = delete;
};

template<class Adaptor, class... Args>
    requires adaptor_has_simple_extra_args<Adaptor, Args...> && (std::is_trivially_copyable_v<Args> && ...)
struct partial<Adaptor, Args...> : range_adaptor_closure<partial<Adaptor, Args...>> {
    std::tuple<Args...> args;

    constexpr partial(Args... _args) noexcept(NO_EXCEPT) : args(std::move(_args)...) {}

    template<class Range>
        requires adaptor_invocable<Adaptor, Range, const Args &...>
    inline constexpr auto operator()(Range&& range) const noexcept(NO_EXCEPT) {
        const auto forwarder = [&range](const auto &..._args) constexpr noexcept(NO_EXCEPT) {
            return Adaptor{}(std::forward<Range>(range), _args...);
        };
        return std::apply(forwarder, this->args);
    }

    static constexpr bool has_simple_call_op = true;
};

template<class Adaptor, class Arg>
    requires adaptor_has_simple_extra_args<Adaptor, Arg> &&
             std::is_trivially_copyable_v<Arg>
struct partial<Adaptor, Arg> : range_adaptor_closure<partial<Adaptor, Arg>> {
    Arg arg;

    constexpr partial(Arg _arg) noexcept(NO_EXCEPT) : arg(std::move(_arg)) {}

    template<class Range>
        requires adaptor_invocable<Adaptor, Range, const Arg &>
    inline constexpr auto operator()(Range&& range) const noexcept(NO_EXCEPT) {
        return Adaptor{}(std::forward<Range>(range), this->arg);
    }

    static constexpr bool has_simple_call_op = true;
};

template<class Lhs, class Rhs, class Range>
concept pipe_invocable = requires {
    std::declval<Rhs>()(std::declval<Lhs>()(std::declval<Range>()));
};

template<class Lhs, class Rhs> struct pipe : range_adaptor_closure<pipe<Lhs, Rhs>> {
    [[no_unique_address]] Lhs lhs;
    [[no_unique_address]] Rhs rhs;

    constexpr pipe(Lhs _lhs, Rhs _rhs) noexcept(NO_EXCEPT) : lhs(std::move(_lhs)), rhs(std::move(_rhs)) {}

    template<class Range>
        requires pipe_invocable<const Lhs &, const Rhs &, Range>
    inline constexpr auto operator()(Range&& range) const & noexcept(NO_EXCEPT) {
        return rhs(lhs(std::forward<Range>(range)));
    }

    template<class Range>
        requires pipe_invocable<Lhs, Rhs, Range>
    inline constexpr auto operator()(Range&& range) && noexcept(NO_EXCEPT) {
        return std::move(rhs)(std::move(lhs)(std::forward<Range>(range)));
    }

    template<class Range>
    inline constexpr auto operator()(Range&& range) const && = delete;
};


template<class Lhs, class Rhs>
    requires closure_has_simple_call_op<Lhs> && closure_has_simple_call_op<Rhs>
struct pipe<Lhs, Rhs> : range_adaptor_closure<pipe<Lhs, Rhs>> {
    [[no_unique_address]] Lhs lhs;
    [[no_unique_address]] Rhs rhs;

    constexpr pipe(Lhs _lhs, Rhs _rhs) noexcept(NO_EXCEPT) : lhs(std::move(_lhs)), rhs(std::move(_rhs)) {}

    template<class Range>
        requires pipe_invocable<const Lhs &, const Rhs &, Range>
    inline constexpr auto operator()(Range&& range) const noexcept(NO_EXCEPT) {
        return rhs(lhs(std::forward<Range>(range)));
    }

    static constexpr bool has_simple_call_op = true;
};


} // namespace views::adaptor


} // namespace lib
