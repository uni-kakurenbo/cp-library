#pragma once


#include <type_traits>

#include "adaptor/valarray.hpp"
#include "adaptor/map.hpp"

#include "internal/resolving_rank.hpp"


namespace lib {

namespace internal {

namespace auto_holder_impl {


template<class T, bool DYNAMIC, class = std::enable_if_t<std::is_integral_v<T> && not DYNAMIC>>
constexpr int _holder_type(resolving_rank<2>) { return 0; }

template<class T, bool>
constexpr auto _holder_type(resolving_rank<1>) -> decltype(std::unordered_set<T>(), 0){ return 1; }

template<class T, bool>
constexpr int _holder_type(resolving_rank<0>) { return 2; }


template<class T, bool DYNAMIC = false>
constexpr int holder_type = _holder_type<T,DYNAMIC>(resolving_rank<10>{});


template<class,class,int> struct holder {};

template<class T, class U>
struct holder<T, U, 0> : vector<U> {
    using vector<U>::vector;
};

template<class T, class U>
struct holder<T, U, 1> : unordered_map<T, U> {
    using unordered_map<T, U>::unordered_map;

    template<class V> inline void assign(const internal::size_t, const V& v) { this->set_default(v); }
};

template<class T, class U>
struct holder<T, U, 2> : map<T, U> {
    using map<T, U>::map;

    template<class V> inline void assign(const internal::size_t, const V& v) { this->set_default(v); }
};


} // namespace auto_holder_impl

} // namespace internal


template<class T, class U> struct auto_holder : internal::auto_holder_impl::holder<T,U,internal::auto_holder_impl::holder_type<T>> {
    using internal::auto_holder_impl::holder<T,U,internal::auto_holder_impl::holder_type<T>>::holder;
};

template<class T, class U> struct dynamic_auto_holder : internal::auto_holder_impl::holder<T,U,internal::auto_holder_impl::holder_type<T,true>> {
    using internal::auto_holder_impl::holder<T,U,internal::auto_holder_impl::holder_type<T,true>>::holder;
};


} // namespace lib
