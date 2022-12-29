#pragma once

#include <atcoder/modint>

#include <iostream>
#include <string>
#include <vector>
#include <iterator>

#include "internal/resolving_rank.hpp"

template<class Stream = std::istream>
struct Input {
  private:
    template<class T>
    auto _set(Lib::Internal::Rank<2>, T *const val) -> decltype(std::declval<Stream>() >> *val, 0) {
        *this->in >> *val;
        return 0;
    }
    template<class T>
    auto _set(Lib::Internal::Rank<1>, T *const val) -> decltype(std::begin(*val), std::end(*val), 0) {
        (*this)(std::begin(*val), std::end(*val));
        return 0;
    }
    template<class T, atcoder::internal::is_modint_t<T>* = nullptr>
    int _set(Lib::Internal::Rank<0>, T *const val) {
        long long v; std::cin >> v;
        *val = { v };
        return 0;
    }

  protected:
    template<class T>
    Stream *set(T *const val) {
        this->_set(Lib::Internal::Rank<2>{}, val);
        return this->in;
    }

  public:
    using char_type = typename Stream::char_type;

    Stream *in;

    Input(Stream *in = &std::cin) : in(in) {}

    template<class T> inline Input& operator>>(T &s) {
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
