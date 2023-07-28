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


template<class...> struct zip_view;

namespace internal {

namespace view_impl {


template<class,class...> struct zip_iterator;

template<class View, class... Iterators>
struct zip_iterator : iterator_interface<std::tuple<typename std::iterator_traits<Iterators>::value_type...>>, iterator_base {
    using difference_type = size_t;
    using iterator_category = most_primitive_iterator_tag<typename std::iterator_traits<Iterators>::iterator_category...>;

    friend View;

  protected:
    using indices = std::make_index_sequence<sizeof...(Iterators)>;
    using iterator_collection = std::tuple<Iterators...>;

    iterator_collection _iterators;

    auto as_const() const noexcept(NO_EXCEPT) {
        return typename View::const_iterator(this->_iterators);
    }

  public:
    explicit zip_iterator() {}
    explicit zip_iterator(const Iterators... iterators) noexcept(NO_EXCEPT) : _iterators({ iterators... }) {}
    template<class... Is> zip_iterator(const std::tuple<Is...> coll) noexcept(NO_EXCEPT) : _iterators(coll) {}

    inline auto& operator++() noexcept(NO_EXCEPT) {
        std::apply([this](auto&... args) { (++args, ...); }, this->_iterators);
        return *this;
    }
    inline auto operator++(int) noexcept(NO_EXCEPT) {
        const auto res = *this;
        std::apply([this](auto&... args) { (++args, ...); }, this->_iterators);
        return res;
    }

    inline auto& operator--() noexcept(NO_EXCEPT) {
        std::apply([this](auto&... args) { (--args, ...); }, this->_iterators);
        return *this;
    }
    inline auto operator--(int) noexcept(NO_EXCEPT) {
        const auto res = *this;
        std::apply([this](auto&... args) { (--args, ...); }, this->_iterators);
        return res;
    }

    inline auto& operator+=(const difference_type diff) noexcept(NO_EXCEPT) {
        std::apply([this,diff](auto&&... args) { ((args += diff), ...); }, this->_iterators);
        return *this;
    }

    inline auto& operator-=(const difference_type diff) noexcept(NO_EXCEPT) {
        std::apply([this,diff](auto&... args) { ((args -= diff), ...); }, this->_iterators);
        return *this;
    }

    friend inline zip_iterator operator+(zip_iterator lhs, const difference_type rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
    friend inline zip_iterator operator-(zip_iterator lhs, const difference_type rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }

    protected:
    template<std::size_t... I>
    const auto dereference_elements_impl(std::index_sequence<I...>) noexcept(NO_EXCEPT) { return std::tie(*std::get<I>(this->_iterators)...); }
    template<std::size_t... I>
    const auto dereference_elements_impl(std::index_sequence<I...>) const noexcept(NO_EXCEPT) { return std::tie(*std::get<I>(this->_iterators)...); }

    const auto dereference_elements() noexcept(NO_EXCEPT) { return dereference_elements_impl(indices()); }
    const auto dereference_elements() const noexcept(NO_EXCEPT) { return dereference_elements_impl(indices()); }

    public:
    const auto operator*() noexcept(NO_EXCEPT) { return dereference_elements(); }
    const auto operator*() const noexcept(NO_EXCEPT) { return dereference_elements(); }

    // For range base for.
    friend inline bool operator==(const zip_iterator &lhs, const zip_iterator &rhs) noexcept(NO_EXCEPT) { return lhs._iterators == rhs._iterators; }
    friend inline bool operator!=(const zip_iterator &lhs, const zip_iterator &rhs) noexcept(NO_EXCEPT) { return lhs._iterators != rhs._iterators; }
    friend inline bool operator<=(const zip_iterator &lhs, const zip_iterator &rhs) noexcept(NO_EXCEPT) { return lhs._iterators <= rhs._iterators; }
    friend inline bool operator>=(const zip_iterator &lhs, const zip_iterator &rhs) noexcept(NO_EXCEPT) { return lhs._iterators >= rhs._iterators; }
    friend inline bool operator<(const zip_iterator &lhs, const zip_iterator &rhs) noexcept(NO_EXCEPT) { return lhs._iterators < rhs._iterators; }
    friend inline bool operator>(const zip_iterator &lhs, const zip_iterator &rhs) noexcept(NO_EXCEPT) { return lhs._iterators > rhs._iterators; }
};

} // namespace view_impl

} // namespace internal



// Thanks to: https://qiita.com/pshiko/items/aee2641149b0cc97e287
template<class... Views>
struct zip_view : internal::view_impl::base {
  public:
    using iterator = internal::view_impl::zip_iterator<zip_view, internal::iterator_t<Views>...>;
    using const_iterator = internal::view_impl::zip_iterator<zip_view, internal::iterator_t<const Views>...>;

  protected:
    std::tuple<Views...> _bases;
    iterator _begin, _end;

  public:
    explicit zip_view(const Views... views) noexcept(NO_EXCEPT) : _bases({ views... }) {
        std::apply(
            [this](const auto&... args) {
                this->_begin = iterator{ std::begin(args)... };
                this->_end = iterator{ std::end(args)... };
            },
            _bases
        );
    };

    iterator begin() noexcept(NO_EXCEPT) { return this->_begin; }
    iterator end() noexcept(NO_EXCEPT) { return this->_end; }

    const_iterator begin() const noexcept(NO_EXCEPT) { return this->_begin.as_const(); }
    const_iterator end() const noexcept(NO_EXCEPT) { return this->_end.as_const(); }

    const_iterator cbegin() const noexcept(NO_EXCEPT) { return this->_begin.as_const(); }
    const_iterator cend() const noexcept(NO_EXCEPT) { return this->_end.as_const(); }
};


namespace views {


template<class... Views>
inline auto zip(Views&&... views) noexcept(NO_EXCEPT) { return zip_view(range(views)...); };


} // namespace views


} // namespace lib
