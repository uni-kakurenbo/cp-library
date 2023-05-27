#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/bitxor.hpp"



namespace lib {

namespace actions {


template<class T> struct range_bitxor : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::bitxor<T>;
};

} // namespace actions


template<class T> struct fenwick_tree<actions::range_bitxor<T>> : internal::fenwick_tree_impl::core<actions::range_bitxor<T>,void> {
  private:
    using base = internal::fenwick_tree_impl::core<actions::range_bitxor<T>,void>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct point_reference : base::point_reference {
        using base::point_reference::point_reference;

        inline point_reference& operator^=(const T& val) noexcept(DEV_ENV) { this->_super->apply(this->_pos, val); return *this; }
    };

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline auto bitxor() const noexcept(NO_EXCEPT) { return this->_super->fold(this->_begin, this->_end); }
    };

    inline point_reference operator[](const size_type p) noexcept(DEV_ENV) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) noexcept(DEV_ENV) { return range_reference(this, l, r); }
};


} // namespace lib
