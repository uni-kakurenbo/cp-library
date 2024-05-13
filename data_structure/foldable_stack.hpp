#pragma once


#include <concepts>
#include <stack>


#include "internal/dev_env.hpp"
#include "internal/unconstructible.hpp"

#include "adaptor/stack.hpp"
#include "adaptor/queue_by_stack.hpp"
#include "adaptor/deque_by_stack.hpp"

#include "algebraic/internal/concepts.hpp"
#include "algebraic/opposite.hpp"

#include "action/base.hpp"


namespace uni {



template<class, template<class...> class = stack>
struct foldable_stack : internal::unconstructible {};


template<algebraic::internal::monoid Monoid, template<class...> class Stack>
struct foldable_stack<Monoid, Stack> {
    using fold_type = Monoid;
    using value_type = Monoid::value_type;

    using size_type = Stack<Monoid>::size_type;

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


template<actions::internal::operatable_action Action, template<class...> class Stack>
struct foldable_stack<Action, Stack> : foldable_stack<typename Action::operand> {
    using foldable_stack<typename Action::operand>::foldable_stack;
};


} // namespace uni
