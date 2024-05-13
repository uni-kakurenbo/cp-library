#pragma once


#include "internal/dev_env.hpp"
#include "internal/unconstructible.hpp"

#include "adaptor/stack.hpp"
#include "adaptor/queue_by_stack.hpp"

#include "algebraic/internal/concepts.hpp"
#include "algebraic/opposite.hpp"

#include "action/base.hpp"

#include "data_structure/foldable_stack.hpp"


namespace uni {


namespace internal {


template<algebraic::internal::monoid Monoid, template<class...> class Stack>
using foldable_queue_base = queue_by_stack<foldable_stack<Monoid, Stack>, foldable_stack<algebraic::make_opposite_t<Monoid>, Stack>>;


} // namespace internal


template<class, template<class...> class = stack>
struct foldable_queue : internal::unconstructible {};


template<algebraic::internal::monoid Monoid, template<class...> class Stack>
struct foldable_queue<Monoid, Stack> : internal::foldable_queue_base<Monoid, Stack> {
    using internal::foldable_queue_base<Monoid, Stack>::foldable_queue_base;

    inline auto fold() const noexcept(NO_EXCEPT) {
        return Monoid(this->_out.fold()) + this->_in.fold();
    }
};


template<actions::internal::operatable_action Action, template<class...> class Stack>
struct foldable_queue<Action, Stack> : foldable_queue<typename Action::operand> {
    using foldable_queue<typename Action::operand>::foldable_queue;
};


} // namespace uni
