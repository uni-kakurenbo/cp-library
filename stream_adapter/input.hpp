#pragma once

#include <iostream>
#include <string>
#include <vector>

template<class Stream = std::istream>
struct Input {
  private:
    template<class T>
    auto _set(int, T * const val) -> decltype(declval<Stream>() >> *val, 0) {
        *this->in >> *val;
        return 0;
    }
    template<class T>
    int _set(bool, T * const val) {
        (*this)(std::begin(*val), std::end(*val));
        return 0;
    }

  protected:
    template<class T>
    Stream *set(T * const val) {
        this->_set(0, val);
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

    template<class T> inline void operator()(T* const val) {
        *this >> *val;
    }

    template<class T, class ...Args> inline void operator()(T* const head, Args* const ...tail) {
        *this >> *head;
        (*this)(tail...);
    }

    template<class I, class = typename I::value_type> inline void operator()(I first, I last) {
        for(I itr=first; itr!=last; ++itr) *this >> *itr;
    }

    template<class F, class S> inline void operator()(std::pair<F,S> * const p) {
        (*this)(&p->first, &p->second);
    }
};
