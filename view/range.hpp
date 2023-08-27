#pragma once


#include <iterator>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/type_traits.hpp"
#include "internal/iterator.hpp"

#include "view/internal/base.hpp"


namespace lib {

namespace internal {

namespace view_impl {

template<class I>
struct range_view : internal::view_impl::base {
    using size_type = internal::size_t;
    using value_type = typename std::iterator_traits<I>::value_type;

    using iterator = I;

  protected:
    I _first, _last;
    mutable size_type _size = -1;

    range_view() = default;

  public:
    explicit range_view(I first, I last) noexcept(NO_EXCEPT) : _first(first), _last(last) { }

    inline size_type size() const noexcept(NO_EXCEPT) {
        if(this->_size < 0) this->_size = std::distance(this->_first, this->_last);
        return this->_size;
    }

    inline auto begin() noexcept(NO_EXCEPT) { return this->_first; }
    inline auto end() noexcept(NO_EXCEPT) { return this->_last; }

    inline auto begin() const noexcept(NO_EXCEPT) { return this->_first; }
    inline auto end() const noexcept(NO_EXCEPT) { return this->_last; }

    inline auto cbegin() const noexcept(NO_EXCEPT) { return this->_first; }
    inline auto cend() const noexcept(NO_EXCEPT) { return this->_last; }

    inline auto operator[](const size_type i) noexcept(NO_EXCEPT) { return this->begin()[i]; }
    inline const auto& operator[](const size_type i) const noexcept(NO_EXCEPT) { return this->begin()[i]; }
};


} // namespace view_impl


template<class,bool,bool> struct _range_view;

template<class Itr> struct _range_view<Itr,false,false> : internal::view_impl::range_view<Itr> {
  protected:
    using base = internal::view_impl::range_view<Itr>;
    using base::base;

    static_assert(internal::is_iterator_v<Itr>);
};

#if CPP20

template<class Range> struct _range_view<Range,true,false> : _range_view<std::ranges::iterator_t<Range>,false,false> {
  protected:
    using base = _range_view<std::ranges::iterator_t<Range>,false,false>;
    static_assert(internal::is_iterable_v<Range>);

  public:
    explicit _range_view(Range& view) : base(std::begin(view), std::end(view)) {}
};

template<class Range> struct _range_view<Range,true,true> : _range_view<std::ranges::iterator_t<Range>,false,false> {
  protected:
    using base = _range_view<std::ranges::iterator_t<Range>,false,false>;
    static_assert(internal::is_iterable_v<Range>);

  public:
    Range _base;
    explicit _range_view(const Range view) : _base(view) {
        // debug(_base);
        this->base::_first = std::begin(this->_base);
        this->base::_last = std::end(this->_base);
    }
};

#else

template<class Range> struct _range_view<Range,true,false> : _range_view<iterator_t<Range>,false,false> {
  protected:
    using base = _range_view<iterator_t<Range>,false,false>;
    static_assert(internal::is_iterable_v<Range>);

  public:
    explicit _range_view(Range& view) : base(std::begin(view), std::end(view)) {}
};

template<class Range> struct _range_view<Range,true,true> : _range_view<iterator_t<Range>,false,false> {
  protected:
    using base = _range_view<iterator_t<Range>,false,false>;
    static_assert(internal::is_iterable_v<Range>);

  public:
    Range _base;
    explicit _range_view(const Range view) : _base(view) {
        // debug(_base);
        this->base::_first = std::begin(this->_base);
        this->base::_last = std::end(this->_base);
    }
};

#endif

} // namespace internal

template<class ItrOrRange> struct range_view : internal::_range_view<ItrOrRange,internal::is_iterable_v<ItrOrRange>,internal::view_impl::is_view_v<ItrOrRange>> {
  protected:
    using base = internal::_range_view<ItrOrRange,internal::is_iterable_v<ItrOrRange>,internal::view_impl::is_view_v<ItrOrRange>>;
    using base::base;
};


template<class ItrOrRange, internal::is_iterator_t<ItrOrRange>* = nullptr>
explicit range_view(const ItrOrRange, const ItrOrRange) -> range_view<ItrOrRange>;

template<class ItrOrRange, internal::view_impl::is_view_t<ItrOrRange>* = nullptr>
explicit range_view(const ItrOrRange) -> range_view<ItrOrRange>;

template<class ItrOrRange, std::enable_if_t<internal::is_iterable_v<ItrOrRange> && !internal::view_impl::is_view_v<ItrOrRange>>* = nullptr>
explicit range_view(const ItrOrRange&) -> range_view<ItrOrRange>;


namespace views {


template<class Itr>
inline auto range(const Itr& first, const Itr& last) noexcept(NO_EXCEPT) { return range_view(first, last); }

template<class View>
inline auto range(View&& views) noexcept(NO_EXCEPT) { return range_view(std::forward<View>(views)); }


} // namespace views


}
