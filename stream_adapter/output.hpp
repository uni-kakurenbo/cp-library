#pragma once

#include <iostream>
#include <string>
#include <vector>

template<class Stream = std::ostream, class Terminator = std::string, class Separator = std::string>
struct Output {
  private:
    template<class T>
    int _put(bool, Stream &ost, const T &val) {
        ost << val;
        return 0;
    }

    template<class T>
    auto _put(int, Stream &ost, const T &val) -> decltype(val.val(), 0) {
        ost << val.val();
        return 0;
    }

  protected:
    template<class T>
    Stream *put(Stream *ost, const T &val) {
        _put(0, *ost, val);
        return out;
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
        put(this->out, s);
        return *this;
    }

    template<class T> inline void operator()(const T &val) {
        *this << val << this->endline;
    }

    template<class T, class ...Args> inline void operator()(const T &head, const Args& ...tail) {
        *this << head << this->separator;
        (*this)(tail...);
    }

    template<class I, class = typename I::value_type> inline void operator()(const I first, const I last) {
        for(I itr=first; itr!=last;) {
            *this << *itr;
            if(++itr == last) *this << this->endline;
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
