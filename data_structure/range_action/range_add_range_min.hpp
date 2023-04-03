#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/minimum.hpp"
#include "data_structure/monoid/addition.hpp"


namespace lib {

namespace actions {


template<class T> struct range_add_range_min : base<monoids::addition<T>> {
    static constexpr flags tags{ flags::implicit_treap, flags::lazy_segment_tree };

    using operand = monoids::minimum<T>;
    using operation = monoids::addition<T>;

    static operand map(const operand& x, const operation& y) { return x.val() + y.val(); }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_add_range_min<T>> : internal::lazy_segment_tree_lib::core<actions::range_add_range_min<T>> {
  private:
    using base = internal::lazy_segment_tree_lib::core<actions::range_add_range_min<T>>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct point_reference : base::point_reference {
        using base::point_reference::point_reference;

        inline point_reference& add(const T& val) { this->_super->apply(this->_pos, val); return *this; }
        inline point_reference& operator+=(const T& val) { this->_super->apply(this->_pos, val); return *this; }
        inline point_reference& operator-=(const T& val) { this->_super->apply(this->_pos, val); return *this; }
    };

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline range_reference& add(const T& val) { this->_super->apply(this->_begin, this->_end, val); return *this; }
        inline range_reference& operator+=(const T& val) { this->_super->apply(this->_begin, this->_end, val); return *this; }
        inline range_reference& operator-=(const T& val) { this->_super->apply(this->_begin, this->_end, val); return *this; }

        inline auto min() const { return this->_super->fold(this->_begin, this->_end); }
    };

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline auto& add(const size_type first, const size_type last, const T& val) { this->apply(first, last, val); return *this; }
    inline auto& add(const size_type pos, const T& val) { this->apply(pos, val); return *this; }

    inline auto& add(const T& val) { this->apply(val); return *this; }
    inline auto& operator+=(const T& val) { this->apply(val); return *this; }
    inline auto& operator-=(const T& val) { this->apply(val); return *this; }

    inline auto min(const size_type first, const size_type last) { return this->fold(first, last); }
    inline auto min() { return this->fold(); }
};



} // namespace lib
