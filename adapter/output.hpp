#pragma once


#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <variant>

#include "internal/dev_env.hpp"
#include "internal/resolving_rank.hpp"


namespace lib {


template<class destination = std::ostream>
struct output_adapter {
  private:
    template<class T>
    auto _put(lib::internal::resolving_rank<4>, const T& val) noexcept(NO_EXCEPT)-> decltype(std::declval<destination&>() << val, 0) {
        *this->out << val;
        return 0;
    }
    template<class T>
    auto _put(lib::internal::resolving_rank<3>, const T& val) noexcept(NO_EXCEPT)-> decltype(val.val(), 0) {
        this->put(val.val());
        return 0;
    }
    template<class T>
    auto _put(lib::internal::resolving_rank<2>, const T& val) noexcept(NO_EXCEPT)-> decltype(std::begin(val), std::end(val), 0) {
        (*this)(std::begin(val), std::end(val), false);
        return 0;
    }
    template<class T>
    auto _put(lib::internal::resolving_rank<1>, const T& val) noexcept(NO_EXCEPT) -> decltype(val.first, val.second, 0) {
        (*this)(val);
        return 0;
    }
    template<class T>
    auto _put(lib::internal::resolving_rank<0>, const T& val) noexcept(NO_EXCEPT) -> decltype(std::get<0>(val), 0) {
        std::apply([this](const auto&... args) constexpr { ((*this << args), ...); }, val);
        return 0;
    }

  protected:
    template<class T>
    destination *put(const T &val) noexcept(NO_EXCEPT){
        this->_put(lib::internal::resolving_rank<10>{}, val);
        return this->out;
    }

  public:
    using char_type = typename destination::char_type;

    static constexpr auto sendl = std::endl<char_type,std::char_traits<char_type>>;
    static constexpr auto sflush = std::flush<char_type,std::char_traits<char_type>>;

  protected:
    using sfunc_type = std::remove_const_t<decltype(output_adapter::sendl)>;

  public:
    using separator_type = std::variant<std::string,sfunc_type>;

    destination *out;
    separator_type endline;
    separator_type separator;

  protected:
    void put_separator() noexcept(NO_EXCEPT) {
        if(this->separator.index() == 0) *this->out << std::get<std::string>(this->separator);
        if(this->separator.index() == 1) *this->out << std::get<sfunc_type>(this->separator);
    }
    void put_endline() noexcept(NO_EXCEPT) {
        if(this->endline.index() == 0) *this->out << std::get<std::string>(this->endline);
        if(this->endline.index() == 1) *this->out << std::get<sfunc_type>(this->endline);
    }

  public:
    template<class Terminator = std::string, class Separator = std::string>
    output_adapter(destination *des = &std::cout, Terminator endl = "\n", Separator sep = " ") noexcept(NO_EXCEPT)
      : out(des), endline(endl), separator(sep)
    {
        *this << std::fixed << std::setprecision(20);
    }

    inline auto& seekp(const typename destination::off_type off, const std::ios_base::seekdir dir = std::ios_base::cur) noexcept(NO_EXCEPT){ this->out->seekp(off, dir); return *this; };

    template<class T> inline output_adapter& operator<<(const T &s) noexcept(NO_EXCEPT){
        this->put(s);
        return *this;
    }

    template<class T = std::string> inline void operator()(const T &val = "") noexcept(NO_EXCEPT){
        *this << val, this->put_endline();
    }

    template<class T, class ...Args> inline void operator()(const T &head, const Args& ...tail) noexcept(NO_EXCEPT){
        *this << head, this->put_separator();
        (*this)(tail...);
    }

    template<class I, class = typename std::iterator_traits<I>::iterator_category> inline void operator()(const I first, const I last, const bool terminate = true) noexcept(NO_EXCEPT){
        for(I itr=first; itr!=last;) {
            *this << *itr;
            if(++itr == last) {
                if(terminate) this->put_endline();
            }
            else this->put_separator();
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


} // namespace lib
