#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>

#include "internal/dev_env.hpp"
#include "internal/resolving_rank.hpp"

template<class destination = std::ostream, class Terminator = std::string, class Separator = std::string>
struct output_adapter {
  private:
    template<class T>
    auto _put(lib::internal::resolving_rank<2>, const T &val) noexcept(NO_EXCEPT)-> decltype(std::declval<destination&>() << val, 0) {
        *this->out << val;
        return 0;
    }
    template<class T>
    auto _put(lib::internal::resolving_rank<1>, const T &val) noexcept(NO_EXCEPT)-> decltype(val.val(), 0) {
        this->put(val.val());
        return 0;
    }
    template<class T>
    auto _put(lib::internal::resolving_rank<0>, const T &val) noexcept(NO_EXCEPT)-> decltype(std::begin(val), std::end(val), 0) {
        (*this)(std::begin(val), std::end(val), false);
        return 0;
    }

  protected:
    template<class T>
    destination *put(const T &val) noexcept(NO_EXCEPT){
        this->_put(lib::internal::resolving_rank<2>{}, val);
        return this->out;
    }

  public:
    using char_type = typename destination::char_type;
    static constexpr auto sendl = std::endl<char_type,std::char_traits<char_type>>;

    destination *out;
    Terminator endline;
    Separator separator;

    output_adapter(destination *out = &std::cout, Terminator endline = "\n", Separator separator = " ") noexcept
      : out(out), endline(endline), separator(separator)
    {
        *this << std::fixed << std::setprecision(20);
    }

    inline void seekp(const typename destination::off_type off, const std::ios_base::seekdir dir = std::ios_base::cur) noexcept(NO_EXCEPT){ this->out->seekp(off, dir); };

    template<class T> inline output_adapter& operator<<(const T &s) noexcept(NO_EXCEPT){
        this->put(s);
        return *this;
    }

    template<class T = std::string> inline void operator()(const T &val = "") noexcept(NO_EXCEPT){
        *this << val << this->endline;
    }

    template<class T, class ...Args> inline void operator()(const T &head, const Args& ...tail) noexcept(NO_EXCEPT){
        *this << head << this->separator;
        (*this)(tail...);
    }

    template<class I, class = typename std::iterator_traits<I>::iterator_category> inline void operator()(const I first, const I last, const bool terminate = true) noexcept(NO_EXCEPT){
        for(I itr=first; itr!=last;) {
            *this << *itr;
            if(++itr == last) {
                if(terminate) *this << this->endline;
            }
            else *this << this->separator;
        }
    }

    template<class T> inline void operator()(const std::initializer_list<T> vals) noexcept(NO_EXCEPT){
        std::vector wrapped(vals.begin(), vals.end());
        (*this)(wrapped.begin(), wrapped.end());
    }

    template<class F, class S> inline void operator()(const std::pair<F,S> &p) noexcept(NO_EXCEPT){
        (*this)(p.first, p.second);
    }
};
