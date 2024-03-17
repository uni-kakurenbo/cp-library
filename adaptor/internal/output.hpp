#pragma once


#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <variant>
#include <ranges>


#include "internal/dev_env.hpp"
#include "internal/resolving_rank.hpp"


namespace lib {


template<class Destination = std::ostream>
struct output_adaptor {
    using destination_type = Destination;

  private:
    template<class T>
        requires
            requires (destination_type& out, T& val) {
                out << val;
            }
    int _put(lib::internal::resolving_rank<5>, T&& val) noexcept(NO_EXCEPT) {
        *this->out << std::forward<T>(val);
        return 0;
    }

    template<class T>
        requires
            requires (T&& val) {
                val.val();
            }
    int _put(lib::internal::resolving_rank<4>, T&& val) noexcept(NO_EXCEPT) {
        this->put(val.val());
        return 0;
    }

    template<std::ranges::input_range T>
    int _put(lib::internal::resolving_rank<3>, T&& val) noexcept(NO_EXCEPT) {
        (*this)(std::ranges::begin(val), std::ranges::end(val), false);
        return 0;
    }

    template<class T>
        requires
            requires (T&& val) {
                val.first;
                val.second;
            }
    int _put(lib::internal::resolving_rank<2>, T&& val) noexcept(NO_EXCEPT) {
        *this << val.first, this->put_separator();
        *this << val.second;
        return 0;
    }
    template<class T>
        requires
            requires (T&& val) {
                std::get<0>(val);
            }
    auto _put(lib::internal::resolving_rank<1>, T&& val) noexcept(NO_EXCEPT) {
        std::apply([this](const auto&... args) constexpr { ((*this << args, this->put_separator()), ...); }, std::forward<T>(val));
        return 0;
    }

    template<std::input_or_output_iterator T>
    int _put(lib::internal::resolving_rank<0>, T&& val) noexcept(NO_EXCEPT) {
        (*this)(*std::forward<T>(val));
        return 0;
    }

  protected:
    template<class T>
    destination_type* put(T&& val) noexcept(NO_EXCEPT){
        this->_put(lib::internal::resolving_rank<10>{}, std::forward<T>(val));
        return this->out;
    }

  public:
    using char_type = typename destination_type::char_type;

    static constexpr auto sendl = std::endl<char_type,std::char_traits<char_type>>;
    static constexpr auto sflush = std::flush<char_type,std::char_traits<char_type>>;

  protected:
    using sfunc_type = std::remove_const_t<decltype(output_adaptor::sendl)>;

  public:
    using separator_type = std::variant<std::string,sfunc_type>;

    destination_type *out;
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
    output_adaptor(destination_type *des = &std::cout, Terminator endl = "\n", Separator sep = " ") noexcept(NO_EXCEPT)
      : out(des), endline(endl), separator(sep)
    {
        *this << std::fixed << std::setprecision(20);
    }

    inline auto& seekp(const typename destination_type::off_type off, const std::ios_base::seekdir dir = std::ios_base::cur) noexcept(NO_EXCEPT) {
        this->out->seekp(off, dir); return *this;
    };

    template<class T> inline output_adaptor& operator<<(T&& s) noexcept(NO_EXCEPT){
        this->put(std::forward<T>(s));
        return *this;
    }

    template<class T = std::string>
    inline auto& operator()(T&& val = "") noexcept(NO_EXCEPT){
        *this << std::forward<T>(val), this->put_endline();
        return *this;
    }

    template<class T, class ...Args>
    inline auto& operator()(T&& head, Args&& ...tail) noexcept(NO_EXCEPT){
        *this << std::forward<T>(head), this->put_separator();
        (*this)(std::forward<Args>(tail)...);
        return *this;
    }

    template<std::forward_iterator I, std::sentinel_for<I> S>
    inline auto& operator()(I first, S last, const bool terminate = true) noexcept(NO_EXCEPT) {
        for(I itr=first; itr!=last;) {
            *this << *itr;
            if(++itr == last) {
                if(terminate) this->put_endline();
            }
            else this->put_separator();
        }

        return *this;
    }

    template<class T>
    inline auto& operator()(const std::initializer_list<T> vals) noexcept(NO_EXCEPT) {
        std::vector wrapped(vals.begin(), vals.end());
        (*this)(wrapped.begin(), wrapped.end());
        return *this;
    }

    template<class T0, class T1>
    inline auto& conditional(const bool cond, const T0& a, const T1& b) noexcept(NO_EXCEPT) {
        if(cond) (*this)(a);
        else (*this)(b);
        return *this;
    }

    inline auto& yesno(const bool cond) noexcept(NO_EXCEPT) {
        if(cond) this->yes();
        else this->no();
        return *this;
    }

    inline auto yes() noexcept(NO_EXCEPT) {
        *this->out << "Yes";
        this->put_endline();
        return *this;
    }

    inline auto no() noexcept(NO_EXCEPT) {
        *this->out << "No";
        this->put_endline();
        return *this;
    }
};


} // namespace lib
