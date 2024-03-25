#pragma once


#include "internal/dev_env.hpp"


namespace lib {


template<class Stack>
struct queue_by_stack {
    using stack_type = Stack;

    using value_type = typename Stack::value_type;
    using size_type = typename Stack::size_type;

  private:
    stack_type _in, _out;

    inline void _shift() noexcept(NO_EXCEPT) {
        if(this->_out.empty()) {
            while(!this->_in.empty()) {
                this->_out.push(this->_in.top());
                this->_in.pop();
            }
        }
    }

  public:
    queue_by_stack() noexcept = default;

    inline bool empty() const noexcept(NO_EXCEPT) {
        return this->_in.empty() && this->_out.empty();
    }

    inline size_type size() const noexcept(NO_EXCEPT) {
        return this->_in.size() + this->_out.size();
    }


    inline value_type front() noexcept(NO_EXCEPT) {
        this->_shift();
        assert(!this->_out.empty());
        return this->_out.top();
    }

    template<std::convertible_to<value_type> T>
        requires std::is_move_constructible_v<T>
    inline value_type front_or(T&& val) noexcept(NO_EXCEPT) {
        if(this->empty()) return static_cast<value_type>(std::forward<T>(val));
        else return this->front();
    }


    inline void clear() noexcept(NO_EXCEPT) {
        this->_in.reset();
        this->_out.reset();
    }


    template<std::convertible_to<value_type> T>
    inline void push(T&& val) noexcept(NO_EXCEPT) { this->_in.push(std::forward<T>(val)); }

    inline void pop() noexcept(NO_EXCEPT) {
        this->_shift();
        assert(!this->_out.empty());
        this->_out.pop();
    }
};



} // namespace lib
