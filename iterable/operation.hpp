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
#include "adapter/vector.hpp"
#include "view/concat.hpp"
#include "constants.hpp"


namespace lib {


template<std::input_iterator I, std::sentinel_for<I> S>
std::string join(const I first, S last, const char* sep = "") noexcept(NO_EXCEPT) {
    if(first == last) return "";
    std::advance(last, -1);
    std::ostringstream res;
    std::copy(first, last, std::ostream_iterator<std::iter_value_t<I>>(res, sep));
    res << *last;
    return res.str();
}

template<std::ranges::input_range R>
std::string join(const R& range, const char* sep = "") noexcept(NO_EXCEPT) {
    return join(ALL(range), sep);
}


template<std::ranges::input_range R0, std::ranges::input_range R1>
    requires std::constructible_from<
        R0, std::common_type_t<std::ranges::range_size_t<R0>,std::ranges::range_size_t<R1>>
    >
R0 concat(const R0& r0, const R1& r1) noexcept(NO_EXCEPT) {
    R0 res(std::ranges::size(r0) + std::ranges::size(r1));
    std::ranges::copy(r0, std::ranges::begin(res));
    std::ranges::copy(r1, std::ranges::next(std::ranges::begin(res), std::ranges::size(r0)));
    return res;
}

template<std::ranges::input_range R, std::ranges::input_range... Rs>
R concat(const R& range, const Rs&... tails) noexcept(NO_EXCEPT) {
    return lib::concat(range, lib::concat(tails...));
}


template<class I, class T = std::iter_value_t<I>>
    requires std::sentinel_for<I, I>
T sum(const I first, const I last, const T& base = {}) noexcept(NO_EXCEPT) {
    return std::accumulate(first, last, base);
}

template<std::ranges::common_range R, class T = std::ranges::range_value_t<R>>
auto sum(const R& range, T base = {}) noexcept(NO_EXCEPT) {
    return sum(ALL(range), base);
}

template<std::ranges::input_range R>
    requires
        requires(R r) {
            r.erase(std::ranges::unique(ALL(r)), std::ranges::end(r));
        }
inline auto unique(R range) noexcept(NO_EXCEPT) {
    std::ranges::sort(range);
    range.erase(std::ranges::unique(ALL(range)), std::ranges::end(range));
    return range;
}


template<
    std::input_iterator I,
    std::sentinel_for<I> S,
    class T = std::iter_value_t<I>
>
T mex(const I first, const S last, const T& base = {}) noexcept(NO_EXCEPT) {
    std::vector<T> val(first, last);
    std::ranges::sort(val);
    val.erase(std::ranges::unique(ALL(val)), val.end());
    val.erase(val.begin(), std::ranges::lower_bound(val, base));

    std::ranges::range_size_t<std::vector<T>> i = 0;
    while(i < std::ranges::size(val) and val[i] == T{i} + base) ++i;

    return T{i} + base;
}

template<class T>
auto mex(const std::initializer_list<T> v) noexcept(NO_EXCEPT) {
    return mex(ALL(v));
}

template<class T>
auto mex(const std::initializer_list<T> v, const T& base) noexcept(NO_EXCEPT) {
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

template<class I, class T = typename std::iterator_traits<I>::value_type>
inline constexpr auto min(I first, I last) noexcept(NO_EXCEPT) {
    T res = std::numeric_limits<T>::max();
    for(auto itr=first; itr!=last; ++itr) res = std::min(res, *itr);
    return res;
}

template<class I, class T = typename std::iterator_traits<I>::value_type>
inline constexpr auto max(I first, I last) noexcept(NO_EXCEPT) {
    T res = std::numeric_limits<T>::lowest();
    for(auto itr=first; itr!=last; ++itr) res = std::max(res, *itr);
    return res;
}


template<std::ranges::range R, class T = std::ranges::range_value_t<R>>
auto min(const R& range) noexcept(NO_EXCEPT) {
    return std::valarray(ALL(range)).min();
}

template<std::ranges::range R, class T = std::ranges::range_value_t<R>>
auto max(const R& range) noexcept(NO_EXCEPT) {
    return std::valarray(ALL(range)).max();
}

template<std::ranges::range R, class T = std::ranges::range_value_t<R>>
auto mex(const R& range, const T& base) noexcept(NO_EXCEPT) {
    return mex(ALL(range), base);
}

template<std::ranges::range R, class T = std::ranges::range_value_t<R>>
auto gcd(const R& range) noexcept(NO_EXCEPT) {
    return gcd(ALL(range));
}

template<std::ranges::range R, class T = std::ranges::range_value_t<R>>
auto lcm(const R& range) noexcept(NO_EXCEPT) {
    return lcm(ALL(range));
}

template<class R, std::input_iterator I, std::sentinel_for<I> S, class D>
    requires
        requires (R r, I itr) {
            r.emplace_back(itr, itr);
        }
auto split(const I first, const S last, const D& delim = ' ') noexcept(NO_EXCEPT) {
    R res;

    for(auto itr=first, fnd=first; ; itr=std::next(fnd)) {
        fnd = std::find(itr, last, delim);
        res.emplace_back(itr, fnd);
        if(fnd == last) break;
    }

    return res;
}

template<class R, std::ranges::input_range V, class D>
    requires (!internal::iterable<D>)
auto split(const V& v, const D& d) noexcept(NO_EXCEPT) { return split<R>(ALL(v), d); }

template<class R, std::ranges::input_range V, internal::iterable Ds>
auto split(const V& v, const Ds& ds) noexcept(NO_EXCEPT) {
    R res = { v };
    ITR(d, ds) {
        R tmp;
        ITR(p, res) tmp = concat(tmp, split<R>(p, d));
        res = std::move(tmp);
    }
    return res;
}

template<class R, std::ranges::input_range V, class T>
auto split(const V& v, const std::initializer_list<T> ds) noexcept(NO_EXCEPT){
    return split<R,V>(v, std::vector<T>(ALL(ds)));
}


template<std::ranges::sized_range R>
auto find(const R source,  const R query) noexcept(NO_EXCEPT) {
    using value_type = std::ranges::range_value_t<R>;

    const auto joined = views::concat(source, query);
    std::vector<value_type> pre_z(std::begin(joined), std::end(joined));
    z_array z_arr(ALL(pre_z));

    const internal::size_t query_size = std::ranges::size(query);

    vector<std::ranges::iterator_t<R>> res;

    {
        auto itr = std::ranges::begin(source);
        REP(i, query_size, z_arr.size()) {
            if(z_arr[i] >= query_size) res.emplace_back(itr);
            ++itr;
        }
    }

    return res;
}

template<std::ranges::input_range R, replacement_policy POLICY = replacement_policy::insert_sync>
auto replace(const R& source, const R& from, const R& to) noexcept(NO_EXCEPT) {
    R res;

    if constexpr(POLICY == replacement_policy::insert_sync) {
        const auto found = find(source, from);
        auto itr = std::begin(source);
        ITRR(fn, found) {
            std::ranges::copy(itr, fn, std::back_inserter(res));
            std::ranges::copy(ALL(to), std::back_inserter(res));
            itr = std::ranges::next(fn, std::size(from));
        }
        std::ranges::copy(itr, std::ranges::end(source), std::back_inserter(res));
    }
    else {
        res = source;
        res.resize(std::size(source) + std::size(to));

        const auto found = find(res, from);
        auto prev = std::ranges::begin(res);
        ITRR(fn, found) {
            if constexpr(POLICY == replacement_policy::overwrite_sync) {
                if(prev <= fn) prev = std::copy(ALL(to), internal::to_non_const_iterator(res, fn));
            }
            else {
                std::copy(ALL(to), internal::to_non_const_iterator(res, fn));
            }
        }

        res.resize(std::size(source));
    }

    return res;
}

template<alignment ALIGNMENT, internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto align(const R& source, const internal::size_t size, const T& v = {}) noexcept(NO_EXCEPT) {
    if(std::ssize(source) >= size) return source;

    if(ALIGNMENT == alignment::left) {
        R left, right;
        left = source;
        right.resize(size - std::size(left), v);
        return R(ALL(lib::views::concat(left, right)));
    }

    if(ALIGNMENT == alignment::center) {
        R left, center, right;
        center = source;
        left.resize((size - std::size(center)) / 2, v);
        right.resize(size - std::size(center) - std::size(left), v);
        return R(ALL(lib::views::concat(left, center, right)));
    }

    if(ALIGNMENT == alignment::right) {
        R left, right;
        right = source;
        left.resize(size - std::size(right), v);
        return R(ALL(lib::views::concat(left, right)));
    }
    assert(false);
}


template<internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto ljust(const R& source, const internal::size_t size, const T& v = {}) noexcept(NO_EXCEPT) {
    return align<alignment::left>(source, size, v);
}

template<internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto cjust(const R& source, const internal::size_t size, const T& v = {}) noexcept(NO_EXCEPT) {
    return align<alignment::center>(source, size, v);
}

template<internal::resizable_range R, class T = std::ranges::range_value_t<R>>
auto rjust(const R& source, const internal::size_t size, const T& v = {}) noexcept(NO_EXCEPT) {
    return align<alignment::right>(source, size, v);
}


} // namespace lib
