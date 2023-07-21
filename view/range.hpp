#pragma once


#include <iterator>

#include "internal/types.hpp"
#include "internal/type_traits.hpp"

#include "view/internal/base.hpp"


namespace lib {


template<class I>
struct range_view : internal::view_impl::base {
    using size_type = internal::size_t;
    using value_type = typename std::iterator_traits<I>::value_type;

    using iterator = I;

  protected:
    I _first, _last;
    size_type _size = -1;

  public:
    explicit range_view(I first, I last) : _first(first), _last(last) { }

    template<class R, std::enable_if_t<internal::view_impl::is_view_v<R>>* = nullptr>
    explicit range_view(R rng) : range_view(std::begin(rng), std::end(rng)) { }

    template<class R, std::enable_if_t<!internal::view_impl::is_view_v<R>>* = nullptr>
    explicit range_view(R& rng) : range_view(std::begin(rng), std::end(rng)) { }

    inline size_type size() const {
        if(this->_size < 0) this->_size = std::distance(this->_first, this->_last);
        return this->_size;
    }

    inline auto begin() { return this->_first; }
    inline auto end() { return this->_last; }

    inline auto begin() const { return this->_first; }
    inline auto end() const { return this->_last; }

    inline auto cbegin() const { return this->_first; }
    inline auto cend() const { return this->_last; }
};


template<class R, std::enable_if_t<internal::view_impl::is_view_v<R>>* = nullptr>
explicit range_view(R rng) -> range_view<internal::iterator_t<R>>;

template<class R, std::enable_if_t<!internal::view_impl::is_view_v<R>>* = nullptr>
explicit range_view(R& rng) -> range_view<internal::iterator_t<R>>;



namespace views {


template<class... Views>
constexpr inline auto range(Views&&... views) noexcept(NO_EXCEPT) { return range_view(views...); }


} // namespace views


}
