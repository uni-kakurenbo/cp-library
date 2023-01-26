#pragma once

#include <atcoder/modint>

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>

#include "internal/resolving_rank.hpp"

template<class source = std::istream>
struct input_adapter {
  private:
    template<class T>
    auto _set(lib::internal::resolving_rank<3>, T *const val) -> decltype(std::declval<source>() >> *val, 0) {
        *this->in >> *val;
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<2>, T *const val) -> decltype(std::begin(*val), std::end(*val), 0) {
        (*this)(std::begin(*val), std::end(*val));
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<1>, T *const val) -> decltype(val->first, val->second, 0) {
        (*this)(&*val);
        return 0;
    }
    template<class T>
    auto _set(lib::internal::resolving_rank<0>, T *const val) -> std::enable_if_t<atcoder::internal::is_modint<T>::value,int> {
        long long v; std::cin >> v;
        *val = { v };
        return 0;
    }

  protected:
    template<class T>
    source *set(T *const val) {
        this->_set(lib::internal::resolving_rank<3>{}, val);
        return this->in;
    }

  public:
    using char_type = typename source::char_type;

    source *in;

    input_adapter(source *in = &std::cin) : in(in) {}

    template<class T> inline input_adapter& operator>>(T &s) {
        this->set(&s);
        return *this;
    }

    template<class T> inline T one() {
        T val; *this >> val;
        return val;
    }

    template<class T> inline void operator()(T*const val) {
        *this >> *val;
    }

    template<class T, class ...Args> inline void operator()(T*const head, Args*const ...tail) {
        *this >> *head;
        (*this)(tail...);
    }

    template<class I, class = typename I::value_type> inline void operator()(I first, I last) {
        for(I itr=first; itr!=last; ++itr) *this >> *itr;
    }

    template<class F, class S> inline void operator()(std::pair<F,S> *const p) {
        (*this)(&p->first, &p->second);
    }
};
