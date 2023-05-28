#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/maximum.hpp"



namespace lib {

namespace actions {


template<class T> struct range_max : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::maximum<T>;
};


} // namespace actions


template<class T> struct segment_tree<actions::range_max<T>> : internal::segment_tree_impl::core<actions::range_max<T>,void> {
  private:
    using base = internal::segment_tree_impl::core<actions::range_max<T>,void>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline auto max() const noexcept(NO_EXCEPT) { return this->_super->fold(this->_begin, this->_end); }
    };

    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto max(const size_t first, const size_t last) noexcept(NO_EXCEPT) { return this->fold(first, last); }
    inline auto max() noexcept(NO_EXCEPT) { return this->fold(); }
};


} // namespace lib
