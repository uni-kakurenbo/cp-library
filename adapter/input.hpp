#pragma once


#include <atcoder/modint>

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>
#include <tuple>

#include "internal/dev_env.hpp"
#include "internal/resolving_rank.hpp"


namespace lib {


template<class source = std::istream>
struct input_adapter {
  private:
    template<class T>
    auto _set(lib::internal::resolving_rank<4>, T *const val) noexcept(NO_EXCEPT) -> decltype(std::declval<source&>() >> *val, 0) {
        *this->in >> *val;
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<3>, T *const val) noexcept(NO_EXCEPT) -> decltype(std::begin(*val), std::end(*val), 0) {
        (*this)(std::begin(*val), std::end(*val));
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<2>, T *const val) noexcept(NO_EXCEPT) -> decltype(val->first, val->second, 0) {
        (*this)(&*val);
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<1>, T *const val) noexcept(NO_EXCEPT) -> decltype(std::get<0>(*val), 0) {
        std::apply([this](auto&... args) constexpr { ((*this >> args), ...); }, *val);
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<0>, T *const val) noexcept(NO_EXCEPT) -> std::enable_if_t<atcoder::internal::is_modint<T>::value,int> {
        long long v; std::cin >> v;
        *val = { v };
        return 0;
    }

  protected:
    template<class T>
    source *set(T& val) noexcept(NO_EXCEPT) {
        this->_set(lib::internal::resolving_rank<10>{}, &val);
        return this->in;
    }
    template<class T>
    source *set(T&& _val) noexcept(NO_EXCEPT) {
        T val = _val;
        this->_set(lib::internal::resolving_rank<10>{}, &val);
        return this->in;
    }

  public:
    using char_type = typename source::char_type;

    source *in;

    input_adapter(source *_in = &std::cin) noexcept(NO_EXCEPT) : in(_in) {}

    template<class T> inline input_adapter& operator>>(T&& s) noexcept(NO_EXCEPT) {
        this->set(std::forward<T>(s));
        return *this;
    }

    template<class T> inline T one() noexcept(NO_EXCEPT) {
        T val; *this >> val;
        return val;
    }

    template<class T> inline void operator()(T*const val) noexcept(NO_EXCEPT) {
        *this >> *val;
    }

    template<class T, class... Args> inline void operator()(T *const head, Args *const... tail) noexcept(NO_EXCEPT) {
        *this >> *head;
        (*this)(tail...);
    }

    template<class I, class = typename std::iterator_traits<I>::value_type> inline void operator()(I first, I last) noexcept(NO_EXCEPT) {
        for(I itr=first; itr!=last; ++itr) *this >> *itr;
    }

    template<class F, class S> inline void operator()(std::pair<F,S> *const p) noexcept(NO_EXCEPT) {
        *this >> p->first >> p->second;
    }
};


} // namespace lib
