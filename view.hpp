#pragma once


#include <cassert>
#include <vector>
#include <iterator>
#include <utility>
#include <type_traits>
#include <functional>
#include <cstring>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"


namespace lib {

namespace internal {

namespace view_impl {


struct base {};


template<class T> using is_view = std::is_base_of<base,T>;
template<class T> inline constexpr auto is_view_v = is_view<T>::value;
template<class T> using is_view_t = std::enable_if_t<is_view_v<T>>;


} // namespace view_impl

} // namespace internal

template<class Ref>
struct view : internal::view_impl::base {
    using size_type = internal::size_t;
    using value_type = typename Ref::value_type;

    using transposer = std::function<size_type(const size_type)>;

  protected:
    transposer _transposer = [&](const size_type pos) { return pos; };

    Ref* _ref;
    size_type _pos = 0, _taken = 0;

  private:
    __attribute__((always_inline)) inline void _validate() const noexcept(DEV_ENV) {
        assert(0 <= this->_pos && this->_pos < static_cast<size_type>(this->_ref->size()));
        assert(0 <= this->_taken);
        assert(this->_pos + this->_taken <= static_cast<size_type>(this->_ref->size()));
    }

  public:
    view(Ref *const ref) noexcept(DEV_ENV) : _ref(ref) {}

    inline view& reset(Ref *const ref) noexcept(DEV_ENV) { this->_ref = ref; return *this; }

    inline Ref* operator->() const noexcept { return this->_ref; }
    inline Ref& operator*() const noexcept { return *this->_ref; }

    inline size_type size() const noexcept(DEV_ENV) { return this->_taken; }

    // inline auto begin() const noexcept(DEV_ENV) { return std::next(std::begin(*this->_ref), this->_pos); }
    // inline auto end() const noexcept(DEV_ENV) { return std::next(this->begin(), this->_taken); }

    inline view& drop(const size_type pos = 0) noexcept(DEV_ENV) { this->_pos = pos; this->_validate(); return *this; }
    inline view& shift(const size_type count = 1) noexcept(DEV_ENV) { this->_pos += count; this->_validate(); return *this; };

    inline view& take(const size_type count) noexcept(DEV_ENV) { this->_taken = count; this->_validate(); return *this; }
    inline view& take() noexcept(DEV_ENV) { this->_taken = std::size(*this->_ref); return *this; }

    inline view& transpose(const transposer& f) noexcept { this->_transposer = f; return *this; }


    inline value_type& operator[](const size_type count) noexcept {
        this->_validate();
        return std::next(std::begin(*this->_ref), this->_pos)[this->_transposer(count)];
    }
    inline const value_type& operator[](const size_type count) const noexcept {
        this->_validate();
        return std::next(std::begin(*this->_ref), this->_pos)[this->_transposer(count)];
    }


  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,view>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const view *const ref, const size_type pos) noexcept(DEV_ENV) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(DEV_ENV) { return this->ref()->operator[](this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const noexcept(DEV_ENV) { return *(*this + count); }
    };

    inline iterator begin() const noexcept(DEV_ENV) { return iterator(this, 0); }
    inline iterator end() const noexcept(DEV_ENV) { return iterator(this, this->size()); }
};

template<class Ref, class Size = std::make_signed_t<std::size_t>>
struct cyclic_view : internal::view_impl::base {
    using size_type = Size;
    using value_type = typename Ref::value_type;

    using transposer = std::function<size_type(const size_type)>;

  protected:
    transposer _transposer = [&](const size_type pos) { return pos; };

    Ref* _ref;
    size_type _pos = 0, _taken;

  private:
    template<class T> __attribute__((always_inline)) inline T _mod(const T v) const noexcept {
        return (v + this->size()) % this->size();
    }

  public:
    cyclic_view(Ref *const ref) noexcept(DEV_ENV) : _ref(ref), _taken(std::size(*ref)) {}

    inline cyclic_view& reset(Ref *const ref) noexcept(DEV_ENV) { this->_ref = ref, this->_taken = std::size(*ref); return *this; }

    inline Ref* operator->() const noexcept { return this->_ref; }
    inline Ref& operator*() const noexcept { return *this->_ref; }

    inline size_type size() const noexcept(DEV_ENV) { return this->_taken; }

    inline cyclic_view& drop(const size_type pos = 0) noexcept(DEV_ENV) { this->_pos = pos; return *this; }
    inline cyclic_view& shift(const size_type count = 1) noexcept(DEV_ENV) { this->_pos += count; return *this; };

    inline cyclic_view& take(const size_type count) noexcept(DEV_ENV) { this->_taken = count; return *this; }
    inline cyclic_view& take() noexcept(DEV_ENV) { this->_taken = std::size(*this->_ref); return *this; }

    inline cyclic_view& transpose(const transposer& f) noexcept { this->_transposer = f; return *this; }


    inline value_type& operator[](const size_type count) noexcept {
        return this->_ref->operator[](this->_mod(this->_pos + this->_transposer(count)));
    }
    inline const value_type& operator[](const size_type count) const noexcept {
        return this->_ref->operator[](this->_mod(this->_pos + this->_transposer(count)));
    }


  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,cyclic_view>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const cyclic_view *const ref, const size_type pos) noexcept(DEV_ENV) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(DEV_ENV) { return this->ref()->operator[](this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const noexcept(DEV_ENV) { return *(*this + count); }
    };

    inline iterator begin() const noexcept(DEV_ENV) { return iterator(this, 0); }
    inline iterator end() const noexcept(DEV_ENV) { return iterator(this, this->size()); }
};

template<class Ref, internal::size_t D>
struct multi_view : internal::view_impl::base {
    using size_type = internal::size_t;

    using transposer = std::function<std::vector<size_type>(const std::initializer_list<size_type>&)>;

  protected:
    transposer _transposer = [&](const std::initializer_list<size_type>& pos) { return std::vector<size_type>(pos.begin(), pos.end()); };

    Ref* _ref;
    size_type _pos[D] = {};

    template<class R, class I, size_type d = 0> inline auto& _access(
        R *const r,
        const I p
    ) noexcept(DEV_ENV) {
        if constexpr(d == D) {
            return *r;
        }
        else {
            size_type index = this->_pos[d] + *std::next(p, d);

            assert(0 <= index && index < static_cast<size_type>(std::size(*r)));

            using nR = std::remove_reference_t<decltype(r->operator[](0))>;
            return multi_view::_access<nR,I,d+1>(&r->operator[](index), p);
        }
    }

  public:
    multi_view(Ref *const ref) noexcept(DEV_ENV) : _ref(ref) {}

    inline multi_view& reset(Ref *const ref) noexcept(DEV_ENV) { this->_ref = ref; return *this; }

    inline Ref* operator->() const noexcept(DEV_ENV) { return this->_ref; }
    inline Ref& operator*() const noexcept(DEV_ENV) { return *this->_ref; }

    inline size_type size() const noexcept(DEV_ENV) { return std::size(*this->_ref); }

    inline multi_view& drop(const std::initializer_list<size_type> pos) noexcept(DEV_ENV) {
        assert(pos.size() == std::size(this->_pos));
        for(auto a=pos.begin(),b=std::begin(this->_pos); a != pos.end(); ++a, ++b) *b = *a;
        return *this;
    }
    template<class... Pos> inline multi_view& drop(const Pos... pos) noexcept(DEV_ENV) { return this->drop({ pos... }); }
    inline multi_view& drop() noexcept(DEV_ENV) { std::memset(this->_pos, 0, sizeof(this->_pos)); return *this; }

    inline const multi_view& transpose(const transposer& f) noexcept(DEV_ENV) { this->_transposer = f; return *this; }


    inline auto& operator[](const std::initializer_list<size_type> pos_) noexcept(DEV_ENV) {
        const auto pos = this->_transposer(pos_);
        return this->_access(this->_ref, pos.begin());
    }
    inline const auto& operator[](std::initializer_list<size_type> pos_) const noexcept(DEV_ENV) {
        const auto pos = this->_transposer(pos_);
        return this->_access(this->_ref, pos.begin());
    }

    template<class... Pos> inline auto& operator()(const Pos... pos) noexcept(DEV_ENV) { return this->operator[]({ pos... }); }
    template<class... Pos> inline const auto& operator()(const Pos... pos) const noexcept(DEV_ENV) { return this->operator[]({ pos... }); }
};

template<class Ref> struct view_2d : internal::view_impl::base {
    using size_type = internal::size_t;

    using transposer = std::function<std::pair<size_type,size_type>(const size_type, const size_type)>;

  protected:
    transposer _transposer = [&](const size_type i, const size_type j) { return { i, j }; };

    Ref* _ref;
    size_type _pos0, _pos1;

  public:
    view_2d(Ref *const ref) noexcept(DEV_ENV) : _ref(ref) {}

    inline view_2d& reset(Ref *const ref) { this->_ref = ref; return *this; }

    inline Ref* operator->() const noexcept(DEV_ENV) { return this->_ref; }
    inline Ref& operator*() const noexcept(DEV_ENV) { return *this->_ref; }

    inline size_type size() const noexcept(DEV_ENV) { return std::size(*this->_ref); }
    inline size_type height() const noexcept(DEV_ENV) { return std::size(*this->_ref); }
    inline size_type width() const noexcept(DEV_ENV) { return std::size(*this->_ref->begin()); }

    inline view_2d& drop(const std::initializer_list<size_type> pos) noexcept(DEV_ENV) {
        assert(pos.size() == std::size(this->_pos));
        for(auto a=pos.begin(),b=std::begin(this->_pos); a != pos.end(); ++a, ++b) *b = *a;
        return *this;
    }
    template<class... Pos> inline view_2d& drop(const Pos... pos) noexcept(DEV_ENV) { return this->drop({ pos... }); }
    inline view_2d& drop() noexcept(DEV_ENV) { std::memset(this->_pos, 0, sizeof(this->_pos)); return *this; }

    inline const view_2d& transpose(const transposer& f) noexcept(DEV_ENV) { this->_transposer = f; return *this; }

    inline auto& operator()(const size_type i, const size_type j) noexcept(DEV_ENV) { return this->operator[]({ i, j }); }
    inline const auto& operator()(const size_type i, const size_type j) const noexcept(DEV_ENV) { return this->operator[]({ i, j }); }

    inline auto& operator[](std::pair<size_type,size_type> pos) noexcept(DEV_ENV) {
        pos = this->_transposer(pos);
        return this->_ref->operator[](pos.first)[pos.second];
    }
    inline const auto& operator[](std::pair<size_type,size_type> pos) const noexcept(DEV_ENV) {
        pos = this->_transposer(pos);
        return this->_ref->operator[](pos.first)[pos.second];
    }
};


}
