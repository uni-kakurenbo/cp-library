#pragma once


#include <concepts>
#include <stack>


#include "internal/dev_env.hpp"

#include "adaptor/stack.hpp"
#include "adaptor/queue_by_stack.hpp"
#include "adaptor/deque_by_stack.hpp"

#include "algebraic/internal/concepts.hpp"
#include "algebraic/opposite.hpp"


namespace uni {


template<algebraic::internal::monoid ValueType, template<class...> class Stack = stack>
struct foldable_stack {
    using fold_type = ValueType;
    using value_type = ValueType::value_type;

    using size_type = Stack<ValueType>::size_type;

    Stack<value_type> _val;
    Stack<fold_type> _acc;


    inline bool empty() const noexcept(NO_EXCEPT) { return this->_val.empty(); }
    inline auto size() const noexcept(NO_EXCEPT) { return this->_val.size(); }


    inline decltype(auto) top() const noexcept(NO_EXCEPT) {
        assert(!this->empty());
        return this->_val.top();
    }

    template<std::convertible_to<value_type> T = value_type>
        requires std::is_move_constructible_v<T>
    inline decltype(auto) top_or(T&& val) const noexcept(NO_EXCEPT) {
        if(this->empty()) return static_cast<value_type>(std::forward<T>(val));
        else return this->top();
    }


    inline auto fold() const noexcept(NO_EXCEPT) {
        if(this->empty()) return fold_type();
        return this->_acc.top();
    }


    template<std::convertible_to<fold_type> T = value_type>
    auto& push(T&& x) noexcept(NO_EXCEPT) {
        const auto acc = this->fold();
        this->_val.push(x);
        this->_acc.push(acc + std::forward<T>(x));
        return *this;
    }

    template<class... Args>
    decltype(auto) emplace(Args&&... args) noexcept(NO_EXCEPT) {
        const auto acc = this->fold();
        decltype(auto) res = this->_val.emplace(std::forward<Args>(args)...);
        this->_acc.push(acc + res);
        return res;
    }

    inline auto& pop() noexcept(NO_EXCEPT) {
        this->_val.pop(), this->_acc.pop();
        return *this;
    }
};


namespace internal {


template<algebraic::internal::monoid Monoid, template<class...> class Stack>
using foldable_queue_base = queue_by_stack<foldable_stack<Monoid, Stack>, foldable_stack<algebraic::make_opposite_t<Monoid>, Stack>>;

template<algebraic::internal::monoid Monoid, template<class...> class Stack>
using foldable_deque_base = deque_by_stack<foldable_stack<algebraic::make_opposite_t<Monoid>, Stack>, foldable_stack<Monoid, Stack>>;


} // namespace internal


template<algebraic::internal::monoid Monoid, template<class...> class Stack = stack>
struct foldable_queue : internal::foldable_queue_base<Monoid, Stack> {
    using internal::foldable_queue_base<Monoid, Stack>::foldable_queue_base;

    inline auto fold() const noexcept(NO_EXCEPT) {
        return Monoid(this->_out.fold()) + this->_in.fold();
    }
};


template<algebraic::internal::monoid Monoid, template<class...> class Stack = stack>
struct foldable_deque : internal::foldable_deque_base<Monoid, Stack> {
    using internal::foldable_deque_base<Monoid, Stack>::foldable_deque_base;

    inline auto fold() const noexcept(NO_EXCEPT) {
        return Monoid(this->_front.fold()) + this->_back.fold();
    }
};


} // namespace uni
