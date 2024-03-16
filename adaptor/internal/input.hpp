#pragma once


#include <atcoder/modint>

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>
#include <tuple>
#include <ranges>
#include <concepts>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/resolving_rank.hpp"

#include "utility/functional.hpp"

#include "numeric/modular/modint_interface.hpp"

#include "adaptor/valarray.hpp"


namespace lib {


template<std::derived_from<std::ios_base> Source = std::istream>
struct input_adaptor {
    using source_type = Source;

  private:
    template<class T>
        requires std::derived_from<T, std::valarray<typename T::value_type>>
    auto _set(lib::internal::resolving_rank<6>, T& val) noexcept(NO_EXCEPT) -> int {
        this->operator()(ALL(val));
        return 0;
    }

    template<class T>
        requires
            requires (source_type& in, T& val) {
                in >> val;
            }
    int _set(lib::internal::resolving_rank<5>, T& val) noexcept(NO_EXCEPT) {
        *this->in >> val;
        return 0;
    }

    template<std::ranges::range T>
    int _set(lib::internal::resolving_rank<4>, T& val) noexcept(NO_EXCEPT) {
        this->operator()(std::ranges::begin(val), std::ranges::end(val));
        return 0;
    }

    template<class T>
        requires
            requires (T& val) {
                val.first;
                val.second;
            }
    int _set(lib::internal::resolving_rank<3>, T& val) noexcept(NO_EXCEPT) {
        *this >> val.first >> val.second;
        return 0;
    }

    template<class T>
        requires
            requires (T& val) {
                std::get<0>(val);
            }
    int _set(lib::internal::resolving_rank<2>, T& val) noexcept(NO_EXCEPT) {
        tuple_for_each([this](auto&& v) { *this >> v; }, val);
        return 0;
    }

    template<lib::internal::modint_family T>
    int _set(lib::internal::resolving_rank<1>, T& val) noexcept(NO_EXCEPT) {
        std::int64_t v; std::cin >> v;
        val = { v };
        return 0;
    }

    template<class T>
        requires
            requires {
                typename T::value_type;
            }
    int _set(lib::internal::resolving_rank<0>, T& val) noexcept(NO_EXCEPT) {
        typename T::value_type v; *this >> v;
        val = { v };
        return 0;
    }

  protected:
    template<class T>
    source_type *set(T& val) noexcept(NO_EXCEPT) {
        this->_set(lib::internal::resolving_rank<10>{}, val);
        return this->in;
    }

    template<class T>
    source_type *set(T&& _val) noexcept(NO_EXCEPT) {
        T val = _val;
        this->_set(lib::internal::resolving_rank<10>{}, val);
        return this->in;
    }

  public:
    using char_type = typename source_type::char_type;

    source_type *in;

    input_adaptor(source_type *_in = &std::cin) noexcept(NO_EXCEPT) : in(_in) {}

    template<class T>
    inline input_adaptor& operator>>(T&& s) noexcept(NO_EXCEPT) {
        this->set(std::forward<T>(s));
        return *this;
    }

    template<class T>
    inline T one() noexcept(NO_EXCEPT) {
        T val; *this >> val;
        return val;
    }

    template<class T>
    inline auto& operator()(T& val) noexcept(NO_EXCEPT) {
        *this >> val;
        return *this;
    }

    template<class T, class... Args>
    inline auto& operator()(T& head, Args&... tail) noexcept(NO_EXCEPT) {
        *this >> head;
        this->operator()(tail...);
        return *this;
    }

    template<std::input_or_output_iterator I, std::sentinel_for<I> S>
    inline auto& operator()(I first, S last) noexcept(NO_EXCEPT) {
        for(I itr=first; itr!=last; ++itr) *this >> *itr;
        return *this;
    }
};


} // namespace lib
