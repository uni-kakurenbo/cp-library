#pragma once


#include <iterator>
#include <utility>
#include <tuple>
#include <type_traits>
#include <functional>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/type_traits.hpp"

#include "view/range.hpp"
#include "view/internal/base.hpp"


namespace lib {


// Thanks to: https://qiita.com/pshiko/items/aee2641149b0cc97e287
template<class... Views>
struct zip_view : internal::view_impl::base {
  protected:
    template<class...> struct iterator_impl;

  public:
    using iterator = iterator_impl<internal::iterator_t<Views>...>;

  protected:
    iterator _begin, _end;

  public:
    constexpr explicit zip_view(Views&... views) noexcept(NO_EXCEPT) : _begin({ std::begin(views)... }), _end({ std::end(views)... }) {};

    iterator& begin() noexcept(NO_EXCEPT) { return this->_begin; }
    const iterator& begin() const noexcept(NO_EXCEPT) { return this->_begin; }

    iterator& end() noexcept(NO_EXCEPT) { return this->_end; }
    const iterator& end() const noexcept(NO_EXCEPT) { return this->_end; }

  protected:
    template<class... Iterators>
    struct iterator_impl : internal::iterator_interface<std::tuple<typename std::iterator_traits<Iterators>::value_type...>> {
        using difference_type = internal::size_t;
        using iterator_category = internal::most_primitive_iterator_tag<typename std::iterator_traits<Iterators>::iterator_category...>;

      protected:
        using indices = std::make_index_sequence<sizeof...(Iterators)>;
        using iterator_collection = std::tuple<Iterators...>;

        iterator_collection _iterators;


        constexpr auto dereference_elements() const noexcept(NO_EXCEPT) { return dereference_elements_impl(indices()); }

        template<std::size_t... I>
        constexpr auto dereference_elements_impl(std::index_sequence<I...>) const noexcept(NO_EXCEPT) { return std::tie(*std::get<I>(this->_iterators)...); }


      public:
        iterator_impl(const Iterators... iterators) noexcept(NO_EXCEPT) : _iterators({ iterators... }) {}

        inline auto& operator++() noexcept(NO_EXCEPT) {
            std::apply([this](auto&... args) constexpr { (++args, ...); }, this->_iterators);
            return *this;
        }
        inline auto operator++(int) noexcept(NO_EXCEPT) {
            const auto res = *this;
            std::apply([this](auto&... args) constexpr { (++args, ...); }, this->_iterators);
            return res;
        }

        inline auto& operator--() noexcept(NO_EXCEPT) {
            std::apply([this](auto&... args) constexpr { (--args, ...); }, this->_iterators);
            return *this;
        }
        inline auto operator--(int) noexcept(NO_EXCEPT) {
            const auto res = *this;
            std::apply([this](auto&... args) constexpr { (--args, ...); }, this->_iterators);
            return res;
        }

        inline auto& operator+=(const difference_type diff) noexcept(NO_EXCEPT) {
            std::apply([this,diff](auto&&... args) constexpr { ((args += diff), ...); }, this->_iterators);
            return *this;
        }

        inline auto& operator-=(const difference_type diff) noexcept(NO_EXCEPT) {
            std::apply([this,diff](auto&... args) constexpr { ((args -= diff), ...); }, this->_iterators);
            return *this;
        }

        friend inline iterator_impl operator+(iterator_impl lhs, const difference_type rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
        friend inline iterator_impl operator-(iterator_impl lhs, const difference_type rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }

        constexpr auto operator*() const noexcept(NO_EXCEPT) { return dereference_elements(); }

        // For range base for.
        friend inline bool operator==(const iterator_impl &lhs, const iterator_impl &rhs) noexcept(NO_EXCEPT) { return lhs._iterators == rhs._iterators; }
        friend inline bool operator!=(const iterator_impl &lhs, const iterator_impl &rhs) noexcept(NO_EXCEPT) { return lhs._iterators != rhs._iterators; }
        friend inline bool operator<=(const iterator_impl &lhs, const iterator_impl &rhs) noexcept(NO_EXCEPT) { return lhs._iterators <= rhs._iterators; }
        friend inline bool operator>=(const iterator_impl &lhs, const iterator_impl &rhs) noexcept(NO_EXCEPT) { return lhs._iterators >= rhs._iterators; }
        friend inline bool operator<(const iterator_impl &lhs, const iterator_impl &rhs) noexcept(NO_EXCEPT) { return lhs._iterators < rhs._iterators; }
        friend inline bool operator>(const iterator_impl &lhs, const iterator_impl &rhs) noexcept(NO_EXCEPT) { return lhs._iterators > rhs._iterators; }
    };
};


namespace views {


template<class... Views>
constexpr inline auto zip(Views&... views) noexcept(NO_EXCEPT) { return zip_view(views...); }


} // namespace views


} // namespace lib
