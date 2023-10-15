#pragma once


#include <iterator>
#include <utility>
#include <tuple>
#include <type_traits>
#include <functional>
#include <ranges>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/type_traits.hpp"

#include "view/internal/base.hpp"



namespace lib {

namespace internal {

namespace view_impl {


template<class,class> struct concat_view;
template<class> struct concat_view_iterator;


template<class View>
inline typename concat_view_iterator<View>::difference_type operator-(
    const concat_view_iterator<View>& lhs,
    const concat_view_iterator<View>& rhs
) noexcept(NO_EXCEPT) {
    if(lhs._block == rhs._block) {
        return lhs._block == 0 ? std::distance(rhs._itr0, lhs._itr0) : std::distance(rhs._itr1, lhs._itr1);
    }
    if(lhs._block > rhs._block) return std::distance(rhs._itr0, rhs._super->_l0) + std::distance(lhs._super->_f1, lhs._itr1);
    if(lhs._block < rhs._block) return -std::distance(lhs, rhs);
    assert(false);
}


template<class View>
struct concat_view_iterator : view_impl::iterator_base {
  protected:
    using iterator0_t = typename View::iterator0_t;
    using iterator1_t = typename View::iterator1_t;

    using ptr_val = std::common_type_t<typename std::iterator_traits<iterator0_t>::pointer, typename std::iterator_traits<iterator1_t>::pointer>;

    using ref_val0_t = typename std::iterator_traits<iterator0_t>::reference;
    using ref_val1_t = typename std::iterator_traits<iterator1_t>::reference;

    using ref_val_t = std::common_reference_t<ref_val0_t,ref_val1_t>;

  public:
    using difference_type = std::common_type_t<typename std::iterator_traits<iterator0_t>::difference_type, typename std::iterator_traits<iterator1_t>::difference_type>;
    using value_type = std::common_type_t<typename std::iterator_traits<iterator0_t>::value_type, typename std::iterator_traits<iterator1_t>::value_type>;
    using pointer = ptr_val;
    using reference = ref_val_t;
    using iterator_category = most_primitive_iterator_tag<typename std::iterator_traits<iterator0_t>::iterator_category, typename std::iterator_traits<iterator1_t>::iterator_category>;

  protected:
    const View* _super;
    bool _block = 0;
    iterator0_t _itr0;
    iterator1_t _itr1;

  public:
    concat_view_iterator() = default;

    template<class Itr>
    explicit concat_view_iterator(
        const View *const super,
        const size_t block,
        Itr itr
    ) noexcept(NO_EXCEPT)
        : _super(super), _block(block)
    {
        if constexpr(std::is_same_v<iterator0_t,iterator1_t>) {
            if(block == 0) this->_itr0 = itr;
            else this->_itr1 = itr;
        }
        else {
            if constexpr(std::is_same_v<iterator0_t,Itr>) {
                assert(block == 0);
                this->_itr0 = itr;
            }
            else {
                assert(block == 1);
                this->_itr1 = itr;
            }
        }
    }

    inline reference operator*() noexcept(NO_EXCEPT) { return this->_block == 0 ? *this->_itr0 : *this->_itr1; }
    inline const reference operator*() const noexcept(NO_EXCEPT) { return this->_block == 0 ? *this->_itr0 : *this->_itr1; }

    inline concat_view_iterator& operator+=(const difference_type count) noexcept(NO_EXCEPT) {
        if(count < 0) return *this -= (-count);

        if(this->_block == 0) {
            const auto dist = static_cast<difference_type>(std::distance(this->_itr0, _super->_l0));
            if(count < dist) {
                this->_itr0 += count;
            }
            else {
                this->_block = 1;
                this->_itr1 = std::next(_super->_f1, dist - count);
            }
        }
        else {
            this->_itr1 += count;
        }

        return *this;
    }

    inline concat_view_iterator& operator-=(const difference_type count) noexcept(NO_EXCEPT) {
        if(count < 0) return *this += (-count);

        if(this->_block == 1) {
            const auto dist = static_cast<difference_type>(std::distance(_super->_f1, this->_itr1));
            if(count < dist) {
                this->_itr1 -= count;
            }
            else {
                this->_block = 0;
                this->_itr0 = std::prev(_super->_l0, dist - count);
            }
        }
        else {
            this->_itr0 -= count;
        }

        return *this;
    }

    inline concat_view_iterator& operator++() noexcept(NO_EXCEPT) {
        if(this->_block == 0) {
            if(++this->_itr0 == _super->_l0) {
                this->_block = 1;
                this->_itr1 = _super->_f1;
            }
        }
        else {
            ++this->_itr1;
        }
        return *this;
    }
    inline concat_view_iterator& operator--() noexcept(NO_EXCEPT) {
        if(this->_block == 1) {
            if(this->_itr1-- == _super->_f1) {
                this->_block = 0;
                this->_itr0 = std::prev(_super->_l0);
            }
        }
        else {
            --this->_itr0;
        }
        return *this;
    }

    inline concat_view_iterator operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; return ++(*this), res; }
    inline concat_view_iterator operator--(int) noexcept(NO_EXCEPT) { const auto res = *this; return --(*this), res; }

    friend inline concat_view_iterator operator+(concat_view_iterator lhs, const difference_type rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
    friend inline concat_view_iterator operator-(concat_view_iterator lhs, const difference_type rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }

    friend inline bool operator==(const concat_view_iterator& lhs, const concat_view_iterator& rhs) noexcept(NO_EXCEPT) {
        if(lhs._block != rhs._block) return false;
        return lhs._block == 0 ? lhs._itr0 == rhs._itr0 : lhs._itr1 == rhs._itr1;
    }
    friend inline bool operator!=(const concat_view_iterator& lhs, const concat_view_iterator& rhs) noexcept(NO_EXCEPT) { return !(lhs == rhs); }

    friend difference_type operator-<View>(const concat_view_iterator&, const concat_view_iterator&);
};


template<class V0, class V1>
struct concat_view : view_impl::base {
    using size_type = std::common_type_t<container_size_t<V0>,container_size_t<V1>>;

  protected:
    using iterator0_t = std::ranges::iterator_t<V0>;
    using iterator1_t = std::ranges::iterator_t<V1>;

    V0 _v0;
    V1 _v1;

    size_type _s0 = -1, _s1 = -1;
    iterator0_t _f0, _l0;
    iterator1_t _f1, _l1;

  public:
    using iterator = concat_view_iterator<concat_view>;
    // using const_iterator = concat_view_iterator<const concat_view>;

    friend iterator;

    friend typename iterator::difference_type operator-<concat_view>(const iterator&, const iterator&);

    using value_type = typename std::iterator_traits<iterator>::value_type;

    explicit concat_view(V0 v0, V1 v1) noexcept(NO_EXCEPT)
      : _v0(std::move(v0)), _v1(std::move(v1)),
        _f0(std::begin(this->_v0)), _l0(std::end(this->_v0)), _f1(std::begin(this->_v1)), _l1(std::end(this->_v1))
    {};

    inline iterator begin() noexcept(NO_EXCEPT) { return iterator(this, 0, this->_f0); }
    inline iterator end() noexcept(NO_EXCEPT) { return iterator(this, 1, this->_l1); }

    inline size_type size() const noexcept(NO_EXCEPT) {
        if(this->_s0 < 0) this->_s0 = std::distance(this->_f0, this->_l0);
        if(this->_s1 < 0) this->_s1 = std::distance(this->_f1, this->_l1);
        return this->_s0 + this->_s1;
    }

    template<class Container>
    inline Container to() const noexcept(NO_EXCEPT) {
        Container res(this->begin(), this->end());
        return res;
    }
};

} // namespace view_impl

} // namespace internal


template<class...> struct concat_view;

template<class V> struct concat_view<V> : std::ranges::ref_view<V> {
    explicit concat_view(V v) noexcept(NO_EXCEPT) : std::ranges::ref_view<V>(v) {}
};

template<class V0, class V1> struct concat_view<V0,V1> : internal::view_impl::concat_view<V0,V1> {
    explicit concat_view(V0 v0, V1 v1) noexcept(NO_EXCEPT) : internal::view_impl::concat_view<V0,V1>(v0, v1) {}
};

template<class V0, class V1, class... Vs> struct concat_view<V0,V1,Vs...> : concat_view<concat_view<V0,V1>,Vs...> {
  public:
    explicit concat_view(V0 v0, V1 v1, Vs... cs) noexcept(NO_EXCEPT) : concat_view<concat_view<V0,V1>,Vs...>(concat_view<V0,V1>(v0, v1), cs...) {}
};

template<class V>
explicit concat_view(V) -> concat_view<V>;

template<class V0, class V1>
explicit concat_view(V0, V1) -> concat_view<V0,V1>;

template<class V0, class V1, class... Vs>
explicit concat_view(V0, V1, Vs...) -> concat_view<V0,V1,Vs...>;


namespace views {


template<class... Range>
inline auto concat(Range&&... range) noexcept(NO_EXCEPT) { return concat_view(std::views::all(range)...); };


} // namespace views


} // namespace lib


#if CPP20


namespace std::ranges {


template<class... Vs>
inline constexpr bool enable_borrowed_range<lib::concat_view<Vs...>> = true;


} // namespace std::ranges


#endif
