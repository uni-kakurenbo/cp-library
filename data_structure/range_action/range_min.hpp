#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/minimum.hpp"



namespace lib {

namespace actions {


template<class T> struct range_min : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::minimum<T>;
};


} // namespace actions


template<class T> struct segment_tree<actions::range_min<T>> : internal::segment_tree_impl::core<actions::range_min<T>,void> {
  private:
    using base = internal::segment_tree_impl::core<actions::range_min<T>,void>;

  public:
    using base::base;
    using size_type = typename base::size_type;

    struct range_reference : base::range_reference {
        using base::range_reference::range_reference;

        inline auto min() const noexcept(NO_EXCEPT) { return this->_super->fold(this->_begin, this->_end); }
    };

    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto min(const size_t first, const size_t last) noexcept(NO_EXCEPT) { return this->fold(first, last); }
    inline auto min() noexcept(NO_EXCEPT) { return this->fold(); }
};


} // namespace lib
