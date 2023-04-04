#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_sum : base<> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::addition<T>;
    using operation = algebraic::assignment<T>;

    static operand map(const operand& x, const operation& y) { return y->value_or(x.val()); }
    static operation fold(const operation& x, const lib::internal::size_t length) {
        if(x->has_value()) return operation(x->operator*() * length);
        return x;
    }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_set_range_sum<T>> : internal::lazy_segment_tree_impl::core<actions::range_set_range_sum<T>> {
  private:
    using base = internal::lazy_segment_tree_impl::core<actions::range_set_range_sum<T>>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct point_reference : base::point_reference {
        using base::point_reference::point_reference;

        inline point_reference& set(const T& val) { this->_super->apply(this->_pos, val); return *this; }
        inline point_reference& operator=(const T& val) { this->_super->apply(this->_pos, val); return *this; }
    };

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline range_reference& set(const T& val) { this->_super->apply(this->_begin, this->_end, val); return *this; }
        inline range_reference& operator=(const T& val) { this->_super->apply(this->_begin, this->_end, val); return *this; }

        inline auto sum() const { return this->_super->fold(this->_begin, this->_end); }
    };

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline auto& set(const size_type first, const size_type last, const T& val) { this->apply(first, last, val); return *this; }
    inline auto& set(const size_type pos, const T& val) { this->apply(pos, val); return *this; }
    inline auto& set(const T& val) { this->apply(val); return *this; }

    inline auto sum(const size_type first, const size_type last) { return this->fold(first, last); }
    inline auto sum() { return this->fold(); }
};


} // namespace lib
