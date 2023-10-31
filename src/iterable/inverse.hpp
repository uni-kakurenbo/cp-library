#pragma once

#include <iterator>
#include <vector>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/vector.hpp"

#include "auto_holder.hpp"


namespace lib {


template<class T, class V = vector<internal::size_t>, class container = dynamic_auto_holder<T,V>>
struct inverse : container {
    explicit inverse() noexcept(NO_EXCEPT) {}
    template<class R> inverse(const R& range) noexcept(NO_EXCEPT) : inverse(std::begin(range), std::end(range)) {}
    template<class I> inverse(const I first, const I last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(static_cast<typename V::value_type>(std::distance(first,itr)));
    }
};

template<class I>
explicit inverse(const I, const I) -> inverse<typename std::iterator_traits<I>::value_type>;

template<class R>
explicit inverse(const R&) -> inverse<typename R::value_type>;


} // namespace lib
