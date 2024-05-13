#pragma once


#include "internal/dev_env.hpp"
#include "internal/unconstructible.hpp"

#include "adaptor/stack.hpp"
#include "adaptor/deque_by_stack.hpp"

#include "algebraic/internal/concepts.hpp"
#include "algebraic/opposite.hpp"

#include "action/base.hpp"

#include "data_structure/foldable_stack.hpp"


namespace uni {


namespace internal {


template<algebraic::internal::monoid Monoid, template<class...> class Stack>
using foldable_deque_base = deque_by_stack<foldable_stack<algebraic::make_opposite_t<Monoid>, Stack>, foldable_stack<Monoid, Stack>>;


} // namespace internal


template<class, template<class...> class = stack>
struct foldable_deque : internal::unconstructible {};


template<algebraic::internal::monoid Monoid, template<class...> class Stack>
struct foldable_deque<Monoid, Stack> : internal::foldable_deque_base<Monoid, Stack> {
    using internal::foldable_deque_base<Monoid, Stack>::foldable_deque_base;

    inline auto fold() const noexcept(NO_EXCEPT) {
        return Monoid(this->_front.fold()) + this->_back.fold();
    }
};


template<actions::internal::operatable_action Action, template<class...> class Stack>
struct foldable_deque<Action, Stack> : foldable_deque<typename Action::operand> {
    using foldable_deque<typename Action::operand>::foldable_deque;
};


} // namespace uni
