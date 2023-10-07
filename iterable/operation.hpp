#pragma once


#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <iterator>
#include <numeric>

#include "snippet/iterations.hpp"
#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/type_traits.hpp"
#include "internal/exception.hpp"

#include "iterable/z_array.hpp"
#include "adapter/vector.hpp"

#include "constants.hpp"
#include "view/concat.hpp"


#if CPP20

#include <ranges>
#include <concepts>

#include "internal/ranges.hpp"

#endif


namespace lib {


template<class I, class = typename std::iterator_traits<I>::iterator_category>
std::string join(const I first, const I last, const char * sep = "") noexcept(NO_EXCEPT) {
    std::ostringstream res;
    std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<I>::value_type>(res, sep));
    return res.str();
}

template<class V>
std::string join(const V& v, const char * sep = "") noexcept(NO_EXCEPT) {
    return join(ALL(v), sep);
}


template<class V, class U>
V concat(const V& v, const U& u) noexcept(NO_EXCEPT) {
    V res(std::size(v) + std::size(u));
    std::copy(ALL(v), std::begin(res));
    std::copy(ALL(u), std::next(std::begin(res), std::size(v)));
    return res;
}

template<class V, class... Us>
V concat(const V& v, const Us&... tails) noexcept(NO_EXCEPT) {
    return lib::concat(v, lib::concat(tails...));
}


template<class I, class T = typename std::iterator_traits<I>::value_type>
T sum(const I first, const I second, const T& base = {}) noexcept(NO_EXCEPT) {
    return std::accumulate(first, second, base);
}

template<class V, class T = typename V::value_type>
auto sum(const V& v, T base = {}) noexcept(NO_EXCEPT) {
    return sum(ALL(v), base);
}

template<class V>
inline auto unique(V v) noexcept(NO_EXCEPT) {
    std::sort(std::begin(v), std::end(v));
    v.erase(std::unique(std::begin(v), std::end(v)), std::end(v));
    return v;
}


template<
    class I,
    class T = typename std::iterator_traits<I>::value_type,
    class = typename std::iterator_traits<I>::value_type
>
T mex(const I first, const I last, const T& base = {}) noexcept(NO_EXCEPT) {
    std::vector<T> val(first, last);
    std::sort(ALL(val));
    val.erase(std::unique(ALL(val)), val.end());
    val.erase(val.begin(), std::lower_bound(ALL(val), base));

    internal::size_t i = 0;
    while(i < (internal::size_t)val.size() and val[i] == T{i} + base) ++i;

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

#if CPP20

template<std::ranges::range R, class T = std::ranges::range_value_t<R>>
auto mex(const R& range, const T& base) noexcept(NO_EXCEPT) {
    return mex(ALL(range), base);
}

#endif


template<class R, class I, class D>
auto split(const I first, const I last, const D& delim = ' ') noexcept(NO_EXCEPT) {
    R res;

    for(auto itr=first, fnd=first; ; itr=std::next(fnd)) {
        fnd = find(itr, last, delim);
        res.emplace_back(itr, fnd);
        if(fnd == last) break;
    }

    return res;
}

template<class R, class V, class D, std::enable_if_t<!internal::is_iterable_v<D>>* = nullptr>
auto split(const V& v, const D& d) noexcept(NO_EXCEPT) { return split<R>(ALL(v), d); }

template<class R, class V, class Ds, std::enable_if_t<internal::is_iterable_v<Ds>>* = nullptr>
auto split(const V& v, const Ds& ds) noexcept(NO_EXCEPT) {
    R res = { v };
    ITR(d, ds) {
        R tmp;
        ITR(p, res) tmp = concat(tmp, split<R>(p, d));
        res = std::move(tmp);
    }
    return res;
}

template<class R, class V, class T> auto split(const V& v, const std::initializer_list<T> ds) noexcept(NO_EXCEPT){
    return split<R,V>(v, std::vector<T>(ALL(ds)));
}


template<class I>
vector<I> find(const I source_first,  const I source_last, const I query_first, const I query_last) noexcept(NO_EXCEPT) {
    using value_type = typename std::iterator_traits<I>::value_type;

    const auto joined = views::concat(views::range(query_first, query_last), views::range(source_first, source_last));
    std::vector<value_type> pre_z(std::begin(joined), std::end(joined));
    z_array z_arr(ALL(pre_z));

    const internal::size_t query_size = std::distance(query_first, query_last);

    vector<I> res;

    {
        auto itr = source_first;
        REP(i, query_size, z_arr.size()) {
            if(z_arr[i] >= query_size) res.emplace_back(itr);
            ++itr;
        }
    }

    return res;
}

template<class V>
auto find(const V& source, const V& query) noexcept(NO_EXCEPT) { return find(ALL(source), ALL(query)); }


template<class V, replacement_policy POLICY = replacement_policy::insert_sync>
auto replace(const V& source, const V& from, const V& to) noexcept(NO_EXCEPT) {
    V res;

    if constexpr(POLICY == replacement_policy::insert_sync) {
        const auto found = find(source, from);
        auto itr = std::begin(source);
        ITRR(fn, found) {
            std::copy(itr, fn, std::back_inserter(res));
            std::copy(ALL(to), std::back_inserter(res));
            itr = std::next(fn, std::size(from));
        }
        std::copy(itr, std::end(source), std::back_inserter(res));
    }
    else {
        res = source;
        res.resize(std::size(source) + std::size(to));

        const auto found = find(res, from);
        auto prev = std::begin(res);
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


#if CPP20


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


#endif

} // namespace lib
