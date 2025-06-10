#pragma once


#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <sstream>
#include <vector>
#include <valarray>
#include <string>
#include <utility>
#include <iterator>
#include <numeric>
#include <limits>
#include <ranges>
#include <concepts>


#include "snippet/iterations.hpp"
#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/type_traits.hpp"
#include "internal/exception.hpp"
#include "internal/iterator.hpp"
#include "internal/ranges.hpp"

#include "iterable/z_array.hpp"
#include "iterable/internal/operation_base.hpp"

#include "adaptor/vector.hpp"
#include "view/concat.hpp"
#include "global/constants.hpp"


namespace uni {


template<std::ranges::input_range R0, std::ranges::input_range R1>
    requires std::constructible_from<
        R0, std::common_type_t<std::ranges::range_size_t<R0>,std::ranges::range_size_t<R1>>
    >
R0 concat(R0&& r0, R1&& r1) noexcept(NO_EXCEPT) {
    R0 res(std::ranges::size(r0) + std::ranges::size(r1));
    std::ranges::copy(r0, std::ranges::begin(res));
    std::ranges::copy(r1, std::ranges::next(std::ranges::begin(res), std::ranges::size(r0)));
    return res;
}

template<std::ranges::input_range R, std::ranges::input_range... Rs>
R concat(R&& range, Rs&&... tails) noexcept(NO_EXCEPT) {
    return uni::concat(range, uni::concat(tails...));
}


template<std::ranges::input_range R>
    requires
        requires(R&& r) {
            r.erase(ALL(std::ranges::unique(r)));
        }
inline auto unique(R&& range) noexcept(NO_EXCEPT) {
    const auto v = std::ranges::unique(range);
    range.erase(std::ranges::begin(v), std::ranges::end(v));
    return range;
}


template<
    std::input_iterator I,
    std::sentinel_for<I> S,
    class T = std::iter_value_t<I>
>
T mex(I first, S last, const T& base = T()) noexcept(NO_EXCEPT) {
    std::vector<T> val(first, last);
    std::ranges::sort(val);
    {
        auto range = std::ranges::unique(val);
        val.erase(ALL(range));
    }
    val.erase(val.begin(), std::ranges::lower_bound(val, base));

    T i = 0;
    while(i < std::ranges::ssize(val) && val[i] == i + base) ++i;

    return T{i} + base;
}

template<std::ranges::input_range R>
auto mex(R&& range, const std::ranges::range_value_t<R>& base = std::ranges::range_value_t<R>()) noexcept(NO_EXCEPT) {
    return mex(ALL(range), base);
}

template<class T>
auto mex(const std::initializer_list<T> v, const T& base = T()) noexcept(NO_EXCEPT) {
    return mex(ALL(v), base);
}


template<std::input_iterator I, std::sentinel_for<I> S, class T>
inline constexpr auto gcd(I first, S last) noexcept(NO_EXCEPT) {
    T res = T{0};
    for(auto itr=first; itr!=last; ++itr) res = std::gcd(res, *itr);
    return res;
}

template<std::input_iterator I, std::sentinel_for<I> S, class T>
inline constexpr auto lcm(I first, S last) noexcept(NO_EXCEPT) {
    T res = T{1};
    for(auto itr=first; itr!=last; ++itr) res = std::lcm(res, *itr);
    return res;
}


template<std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
auto mex(R&& range, const T& base) noexcept(NO_EXCEPT) {
    return mex(ALL(range), base);
}


template<std::ranges::input_range R>
auto gcd(R&& range) noexcept(NO_EXCEPT) {
    return gcd(ALL(range));
}

template<std::ranges::input_range R>
auto lcm(R&& range) noexcept(NO_EXCEPT) {
    return lcm(ALL(range));
}


template<class R, std::input_iterator I, std::sentinel_for<I> S, class D>
    requires
        requires (R r, I itr) {
            r.emplace_back(itr, itr);
        }
auto split(I first, S last, const D& delim = ' ') noexcept(NO_EXCEPT) {
    R res;

    for(auto itr=first, fnd=first; ; itr=std::ranges::next(fnd)) {
        fnd = std::find(itr, last, delim);
        res.emplace_back(itr, fnd);
        if(fnd == last) break;
    }

    return res;
}


template<class R, std::ranges::input_range V, class D>
    requires (!std::ranges::input_range<D>)
auto split(V&& v, D&& d) noexcept(NO_EXCEPT) { return split<R>(ALL(v), d); }


template<class R, std::ranges::input_range V, std::ranges::input_range Ds>
auto split(V&& v, Ds&& ds) noexcept(NO_EXCEPT) {
    R res = { v };
    ITR(d, ds) {
        R tmp;
        ITR(p, res) tmp = concat(tmp, split<R>(p, d));
        res = std::move(tmp);
    }
    return res;
}


template<class R, std::ranges::input_range V, class T>
auto split(V&& v, const std::initializer_list<T> ds) noexcept(NO_EXCEPT){
    return split<R,V>(v, std::vector<T>(ALL(ds)));
}


template<std::ranges::sized_range Source, std::ranges::sized_range Qeury>
auto find(Source&& source,  Qeury&& query) noexcept(NO_EXCEPT) {
    z_array z_arr(views::concat(query, source));
    const auto query_size = std::ranges::ssize(query);

    vector<std::ranges::iterator_t<Source>> res;
    {
        auto itr = std::ranges::begin(source);
        REP(i, query_size, std::ranges::size(z_arr)) {
            if(z_arr[i] >= query_size) res.push_back(itr);
            ++itr;
        }
    }

    return res;
}


template<
    replacement_policy POLICY,
    std::ranges::sized_range R,
    std::ranges::sized_range From,
    std::ranges::sized_range To
>
auto replaced(R&& source, From&& from, To&& to) noexcept(NO_EXCEPT) {
    std::remove_cvref_t<R> res;

    if constexpr(POLICY == replacement_policy::insert_sync) {
        const auto found = find(source, from);
        auto itr = std::ranges::begin(source);
        ITRR(fn, found) {
            std::ranges::copy(itr, fn, std::back_inserter(res));
            std::ranges::copy(ALL(to), std::back_inserter(res));
            itr = std::ranges::next(fn, std::ranges::size(from));
        }
        std::ranges::copy(itr, std::ranges::end(source), std::back_inserter(res));
    }
    else {
        res = source;
        res.resize(std::ranges::size(source) + std::ranges::size(to));

        const auto found = find(res, from);
        auto prev = std::ranges::begin(res);
        ITRR(fn, found) {
            if constexpr(POLICY == replacement_policy::overwrite_sync) {
                if(prev <= fn) prev = std::ranges::copy(to, fn);
            }
            else {
                std::ranges::copy(to, fn);
            }
        }

        res.resize(std::ranges::size(source));
    }

    return res;
}

template<
    std::ranges::sized_range R,
    std::ranges::sized_range From,
    std::ranges::sized_range To
>
inline auto replaced(R&& source, From&& from, To&& to) noexcept(NO_EXCEPT) {
    return replaced<replacement_policy::insert_sync, R, From, To>(std::forward<R>(source), std::forward<From>(from), std::forward<To>(to));
}


template<alignment ALIGNMENT, internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto align(R&& source, const internal::size_t size, const T& v = T()) noexcept(NO_EXCEPT) {
    if(std::ssize(source) >= size) return source;

    if(ALIGNMENT == alignment::left) {
        R left, right;
        left = source;
        right.resize(size - std::size(left), v);
        return R(ALL(uni::views::concat(left, right)));
    }

    if(ALIGNMENT == alignment::center) {
        R left, center, right;
        center = source;
        left.resize((size - std::size(center)) / 2, v);
        right.resize(size - std::size(center) - std::size(left), v);
        return R(ALL(uni::views::concat(left, center, right)));
    }

    if(ALIGNMENT == alignment::right) {
        R left, right;
        right = source;
        left.resize(size - std::size(right), v);
        return R(ALL(uni::views::concat(left, right)));
    }
    assert(false);
}


template<internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto ljust(R&& source, const internal::size_t size, const T& v = T()) noexcept(NO_EXCEPT) {
    return align<alignment::left>(source, size, v);
}

template<internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto cjust(R&& source, const internal::size_t size, const T& v = T()) noexcept(NO_EXCEPT) {
    return align<alignment::center>(source, size, v);
}

template<internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto rjust(R&& source, const internal::size_t size, const T& v = T()) noexcept(NO_EXCEPT) {
    return align<alignment::right>(source, size, v);
}


template<
    class Res,
    std::ranges::random_access_range Target,
    std::ranges::forward_range Order
>
    requires std::ranges::output_range<Res, std::ranges::range_value_t<Target>>
Res ordered_by(Target&& target, Order&& order) noexcept(NO_EXCEPT) {
    const auto target_size = std::ranges::ssize(target);
    const auto order_size = std::ranges::ssize(order);
    Res res(order_size);

    {
        auto res_itr = std::ranges::begin(res);
        auto order_itr = std::ranges::begin(order);
        const auto order_end = std::ranges::end(std::forward<Order>(order));

        for(; order_itr != order_end; ++res_itr, ++order_itr) {
            if constexpr(std::signed_integral<std::ranges::range_value_t<Order>>) assert(0 <= *order_itr);
            assert(*order_itr < target_size);
            *res_itr = target[*order_itr];
        }
    }

    return res;
}


template<
    std::ranges::random_access_range Target,
    std::ranges::forward_range Order
>
auto ordered_by(Target&& target, Order&& order) noexcept(NO_EXCEPT) {
    return ordered_by<std::remove_cvref_t<Target>, Target, Order>(std::forward<Target>(target), std::forward<Order>(order));
}


template<std::ranges::input_range Target, std::ranges::input_range Source>
    requires std::equality_comparable_with<std::ranges::range_value_t<Target>, std::ranges::range_value_t<Source>>
auto is_subsequence_of(Target&& target, Source&& source) noexcept(NO_EXCEPT) {
    auto target_itr = std::ranges::begin(source);
    auto source_itr = std::ranges::begin(source);

    const auto target_end = std::ranges::end(source);
    const auto source_end = std::ranges::end(source);

    for(; source_itr != source_end; ++source_itr) {
        if(*target_itr == *source_itr) ++target_itr;
    }

    return target_itr == target_end;
}


template<std::ranges::input_range Target, std::ranges::input_range Source>
    requires std::equality_comparable_with<std::ranges::range_value_t<Target>, std::ranges::range_value_t<Source>>
auto is_continuous_subsequence_of(Target&& target, Source&& source) noexcept(NO_EXCEPT) {
    const auto found = find(source, target);
    return found.size() > 0;
}


template<std::ranges::bidirectional_range R>
auto is_palindrome(R&& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::distance(range);

    auto fwd = std::ranges::begin(range);
    auto rev = std::ranges::rbegin(range);

    REP(n / 2) {
        if(*(fwd++) != *(rev++)) return false;
    }

    return true;
}


} // namespace uni
