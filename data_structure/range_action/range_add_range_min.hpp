#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/minimum.hpp"
#include "algebraic/addition.hpp"


namespace lib {

namespace actions {


template<class T> struct range_add_range_min : base<algebraic::addition<T>> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::minimum<T>;
    using operation = algebraic::addition<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) { return x.val() + y.val(); }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_add_range_min<T>> : internal::lazy_segment_tree_impl::core<actions::range_add_range_min<T>> {
  private:
    using base = internal::lazy_segment_tree_impl::core<actions::range_add_range_min<T>>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct point_reference : base::point_reference {
        using base::point_reference::point_reference;

        inline point_reference& add(const T& val) noexcept(NO_EXCEPT) { this->_super->apply(this->_pos, val); return *this; }
        inline point_reference& operator+=(const T& val) noexcept(NO_EXCEPT) { this->_super->apply(this->_pos, val); return *this; }
        inline point_reference& operator-=(const T& val) noexcept(NO_EXCEPT) { this->_super->apply(this->_pos, val); return *this; }
    };

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline range_reference& add(const T& val) noexcept(NO_EXCEPT) { this->_super->apply(this->_begin, this->_end, val); return *this; }
        inline range_reference& operator+=(const T& val) noexcept(NO_EXCEPT) { this->_super->apply(this->_begin, this->_end, val); return *this; }
        inline range_reference& operator-=(const T& val) noexcept(NO_EXCEPT) { this->_super->apply(this->_begin, this->_end, val); return *this; }

        inline auto min() const { return this->_super->fold(this->_begin, this->_end); }
    };

    inline point_reference operator[](const size_type p) noexcept(NO_EXCEPT) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto& add(const size_type first, const size_type last, const T& val) noexcept(NO_EXCEPT) { this->apply(first, last, val); return *this; }
    inline auto& add(const size_type pos, const T& val) noexcept(NO_EXCEPT) { this->apply(pos, val); return *this; }
    inline auto& add(const T& val) noexcept(NO_EXCEPT) { this->apply(val); return *this; }

    inline auto min(const size_type first, const size_type last) noexcept(NO_EXCEPT) { return this->fold(first, last); }
    inline auto min() noexcept(NO_EXCEPT) { return this->fold(); }
};



} // namespace lib
