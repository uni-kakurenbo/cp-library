#pragma once


#include <memory>
#include <type_traits>
#include <memory_resource>


#include "data_structure/internal/node_handler.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {


template<class ValueType, class Allocator = std::allocator<ValueType>>
struct persistent_stack {
    using value_type = ValueType;
    using size_type = internal::size_t;


    struct node_type;
    using node_handler = node_handlers::cloneable<Allocator>::template handler<node_type>;

    using allocator_type = typename node_handler::allocator_type;
    using node_pointer = typename node_handler::node_pointer;

    struct node_type {
        value_type value;
        node_pointer next;

        node_type(value_type _value, node_pointer _next) noexcept(NO_EXCEPT)
          : value(_value), next(_next)
        {}
    };

  private:
    size_type _size = 0;
    node_pointer _head = {};

    [[no_unique_address]] node_handler _node_handler = {};

  public:
    explicit persistent_stack(const allocator_type& allocator = {}) noexcept(NO_EXCEPT) : _node_handler(allocator) {};


    persistent_stack(const persistent_stack& source, const allocator_type& allocator) noexcept(NO_EXCEPT)
      : _size(source._size), _head(source._head), _node_handler(allocator)
    {};

    persistent_stack(persistent_stack&& source, const allocator_type& allocator) noexcept(NO_EXCEPT)
      : _size(source._size), _head(source._head), _node_handler(allocator)
    {};


    inline auto clone() const noexcept(NO_EXCEPT) { return *this; }


    inline bool empty() const noexcept(NO_EXCEPT) { return !this->_head; }
    inline size_type size() const noexcept(NO_EXCEPT) { return this->_size; }

    inline value_type top() const noexcept(NO_EXCEPT) {
        assert(!this->empty());
        return this->_head->value;
    }

    template<std::convertible_to<value_type> T>
        requires std::is_move_constructible_v<T>
    inline value_type top_or(T&& v) const noexcept(NO_EXCEPT) {
        if(this->empty()) return static_cast<value_type>(std::forward<T>(v));
        else return this->top();
    }


    inline auto& clear() noexcept(NO_EXCEPT) {
        this->_head.reset();
        this->_size = 0;
        return *this;
    }


    template<std::convertible_to<value_type> T>
    inline auto& push(T&& x) noexcept(NO_EXCEPT) {
        this->_head = this->_node_handler.create(std::forward<T>(x), this->_head);
        ++this->_size;
        return *this;
    }


    inline auto& pop() noexcept(NO_EXCEPT) {
        assert(!this->empty());
        this->_head = this->_head->next;
        --this->_size;
        return *this;
    }
};


namespace pmr {


template<class T>
using persistent_stack = lib::persistent_stack<T, std::pmr::polymorphic_allocator<T>>;


} // namesapce pmr


} // namespace lib
