#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "internal/resolving_rank.hpp"

template<class Stream = std::ostream, class Terminator = std::string, class Separator = std::string>
struct Output {
  private:
    template<class T>
    auto _put(Lib::Internal::Rank<2>, const T &val) -> decltype(std::declval<Stream>() << val, 0) {
        *this->out << val;
        return 0;
    }
    template<class T>
    auto _put(Lib::Internal::Rank<1>, const T &val) -> decltype(val.val(), 0) {
        *this->out << val.val();
        return 0;
    }
    template<class T>
    auto _put(Lib::Internal::Rank<0>, const T &val) -> decltype(std::begin(val), std::end(val), 0) {
        (*this)(std::begin(val), std::end(val), false);
        return 0;
    }

  protected:
    template<class T>
    Stream *put(const T &val) {
        this->_put(Lib::Internal::Rank<2>{}, val);
        return this->out;
    }

  public:
    using char_type = typename Stream::char_type;
    static constexpr auto sendl = std::endl<char_type,std::char_traits<char_type>>;

    Stream *out;
    Terminator endline;
    Separator separator;

    Output(Stream *out = &std::cout, Terminator endline = "\n", Separator separator = " ")
      : out(out), endline(endline), separator(separator) {}


    template<class T> inline Output& operator<<(const T &s) {
        this->put(s);
        return *this;
    }

    template<class T = std::string> inline void operator()(const T &val = "") {
        *this << val << this->endline;
    }

    template<class T, class ...Args> inline void operator()(const T &head, const Args& ...tail) {
        *this << head << this->separator;
        (*this)(tail...);
    }

    template<class I, class = typename I::iterator_category> inline void operator()(const I first, const I last, const bool terminate = true) {
        for(I itr=first; itr!=last;) {
            *this << *itr;
            if(++itr == last) {
                if(terminate) *this << this->endline;
            }
            else *this << this->separator;
        }
    }

    template<class T> inline void operator()(const std::initializer_list<T> vals) {
        std::vector wrapped(vals.begin(), vals.end());
        (*this)(wrapped.begin(), wrapped.end());
    }

    template<class F, class S> inline void operator()(const std::pair<F,S> &p) {
        (*this)(p.first, p.second);
    }
};
