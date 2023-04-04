#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/affine.hpp"


namespace lib {

namespace actions {


template<class T> struct range_affine_range_sum : base<> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::addition<T>;
    using operation = algebraic::affine<T>;

    static operand map(const operand& x, const operation& y) { return y->first * x.val() + y->second; }
    static operation fold(const operation& x, const lib::internal::size_t length) { return operation({ x->first, x->second * length }); }
};


} // namespace actions


template<class T>
struct lazy_segment_tree<actions::range_affine_range_sum<T>> : internal::lazy_segment_tree_impl::core<actions::range_affine_range_sum<T>> {
  private:
    using base = internal::lazy_segment_tree_impl::core<actions::range_affine_range_sum<T>>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct point_reference : base::point_reference {
        using base::point_reference::point_reference;

        inline point_reference& affine(const T& a, const T& b) { return this->_super->apply(this->_pos, { a, b }); return *this; }

        inline point_reference& set(const T& val) { this->_super->apply(this->_pos, { 0, val }); return *this; }
        inline point_reference& operator=(const T& val) { this->_super->apply(this->_pos, { 0, val }); return *this; }

        inline point_reference& add(const T& val) { this->_super->apply(this->_pos, { 1, val }); return *this; }
        inline point_reference& operator+=(const T& val) { this->_super->apply(this->_pos, { 1, val }); return *this; }

        inline point_reference& times(const T& val) { this->_super->apply(this->_pos, { val, 0 }); return *this; }
        inline point_reference& operator*=(const T& val) { this->_super->apply(this->_pos, { val, 0 }); return *this; }
    };

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline range_reference& affine(const T& a, const T& b) { this->_super->apply(this->_begin, this->_end, { a, b }); return *this; }

        inline range_reference& set(const T& val) { this->_super->apply(this->_begin, this->_end, { 0, val }); return *this; }
        inline range_reference& operator=(const T& val) { this->_super->apply(this->_begin, this->_end, { 0, val }); return *this; }

        inline range_reference& add(const T& val) { this->_super->apply(this->_begin, this->_end, { 1, val }); return *this; }
        inline range_reference& operator+=(const T& val) { this->_super->apply(this->_begin, this->_end, { 1, val }); return *this; }
        inline range_reference& operator-=(const T& val) { this->_super->apply(this->_begin, this->_end, { 1, -val }); return *this; }

        inline range_reference& times(const T& val) { this->_super->apply(this->_begin, this->_end, { val, 0 }); return *this; }
        inline range_reference& operator*=(const T& val) { this->_super->apply(this->_begin, this->_end, { val, 0 }); return *this; }

        inline auto sum() const { return this->_super->fold(this->_begin, this->_end); }
    };

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline auto& affine(const size_t first, const size_t last, const T& a, const T& b) { this->apply(first, last, { a, b }); return *this; }
    inline auto& affine(const size_t pos, const T& a, const T& b) { this->apply(pos, { a, b }); return *this; }
    inline auto& affine(const T& a, const T& b) { this->apply({ a, b }); return *this; }

    inline auto& set(const size_t first, const size_t last, const T& val) { this->apply(first, last, { 0, val }); return *this; }
    inline auto& set(const size_t pos, const T& val) { this->apply(pos, { 0, val }); return *this; }
    inline auto& set(const T& val) { this->apply({ 0, val }); return *this; }

    inline auto& add(const size_type first, const size_type last, const T& val) { this->apply(first, last, { 1, val }); return *this; }
    inline auto& add(const size_type pos, const T& val) { this->apply(pos, { 1, val }); return *this; }
    inline auto& add(const T& val) { this->apply({ 1, val }); return *this; }

    inline auto& times(const size_t first, const size_t last, const T& val) { this->apply(first, last, { val, 0 }); return *this; }
    inline auto& times(const size_t pos, const T& val) { this->apply(pos, { val, 0 }); return *this; }
    inline auto& times(const T& val) { this->apply({ val, 0 }); return *this; }


    inline auto sum(const size_t first, const size_t last) { return this->fold(first, last); }
    inline auto sum() { return this->fold(); }
};


} // namespace lib
