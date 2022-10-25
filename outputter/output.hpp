#pragma once

#include <iostream>
#include <string>


template<class Stream = std::ostream, class Terminator = std::string, class Separator = std::string>
struct Output {
    using char_type = typename Stream::char_type;
    static constexpr auto sendl = std::endl<char_type,std::char_traits<char_type>>;

    Stream *out;
    Terminator endline;
    Separator separator;

    Output(Stream *out = &std::cout, Terminator endline = "\n", Separator separator = " ")
      : out(out), endline(endline), separator(separator) {}


    template<class T> inline void operator()(const T val) {
        *this->out << val << this->endline;
    }

    template<class T, class ...Args> inline void operator()(const T head, const Args ...tail) {
        *this->out << head << this->separator;
        (*this)(tail...);
    }

    template<class I, class = typename I::value_type> inline void operator()(const I first, const I last) {
        for(I itr=first; itr!=last;) {
            *this->out << *itr;
            if(++itr == last) *this->out << this->endline;
            else *this->out << this->separator;
        }
    }

    template<class T> inline void operator()(const std::initializer_list<T> vals) {
        std::vector wrapped(vals.begin(), vals.end());
        (*this)(wrapped.begin(), wrapped.end());
    }

    template<class F, class S> inline void operator()(const std::pair<F,S> p) {
        (*this)(p.first, p.second);
    }

    template<class T> inline Output& operator<<(const T s) {
        (*this)(s);
        return *this;
    }

};
