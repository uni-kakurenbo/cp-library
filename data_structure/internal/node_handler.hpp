#pragma once


#include <memory>

#include "internal/dev_env.hpp"


namespace lib {

namespace node_handlers {

namespace internal {


template<class Allocator, class NodeType>
struct base_handler {
    using allocator_type = Allocator;

  protected:
    using allocator_traits = std::allocator_traits<allocator_type>;

    using node_allocator_type = allocator_traits::template rebind_alloc<NodeType>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    [[no_unique_address]] node_allocator_type _allocator;

  public:
    base_handler(const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
        : _allocator(allocator)
    {}

    base_handler(const base_handler& source) noexcept(NO_EXCEPT)
        : _allocator(node_allocator_traits::select_on_container_copy_construction(source._allocator))
    {}

    base_handler(base_handler&& source) noexcept = default;

    base_handler& operator=(const base_handler& source) noexcept(NO_EXCEPT) {
        if(&source != this) {
            if constexpr(allocator_traits::propagate_on_container_copy_assignment::value) {
                this->_allocator = source._allocator;
            }
        }
        return *this;
    }

    base_handler& operator=(base_handler&& source) noexcept(NO_EXCEPT) {
        if(&source != this) {
            if constexpr(allocator_traits::propagate_on_container_move_assignment::value) {
                this->_allocator = source._allocator;
            }
        }
        return *this;
    }
};


} // namespace internal


template<class Allocator>
struct cloneable {
    template<class NodeType>
    struct handler : internal::base_handler<Allocator, NodeType> {
        using internal::base_handler<Allocator, NodeType>::base_handler;

        using node_type = NodeType;
        using node_pointer = std::shared_ptr<node_type>;

        inline static node_pointer nil = std::make_shared<node_type>();

        template<class... Args>
        inline node_pointer create(Args&&... args) noexcept(NO_EXCEPT) {
            return std::allocate_shared<node_type>(this->_allocator, std::forward<Args>(args)...);
        }

        inline node_pointer clone(const node_pointer& ptr) noexcept(NO_EXCEPT) {
            return this->create(*ptr);
        }

        inline constexpr bool disposable(const node_pointer&) const noexcept { return false; }
        inline constexpr void dispose(const node_pointer&) const noexcept {}
    };
};


template<class Allocator>
struct reusing {
    template<class NodeType>
    struct handler : internal::base_handler<Allocator, NodeType> {
        using node_type = NodeType;
        using node_pointer = std::add_pointer_t<node_type>;

      private:
        using base = internal::base_handler<Allocator, NodeType>;
        using node_allocator_traits = typename base::node_allocator_traits;

        inline static int _instance_count = 0;

      public:
        using base::base;

        using allocator_type = typename base::allocator_type;


        inline static node_pointer nil;


        handler(const allocator_type& allocator = {}) noexcept(NO_EXCEPT) : base(allocator) {
            if(handler::_instance_count++ == 0) {
                handler::nil = new node_type{};
            }
        }

        ~handler() noexcept {
            if(--handler::_instance_count == 0) {
                delete handler::nil;
            }
        }


        template<class... Args>
        inline node_pointer create(Args&&... args) noexcept(NO_EXCEPT) {
            node_pointer node = node_allocator_traits::allocate(this->_allocator, 1);
            node_allocator_traits::construct(this->_allocator, node, std::forward<Args>(args)...);

            return node;
        }

        inline node_pointer clone(const node_pointer ptr) const noexcept { return ptr; }

        inline bool disposable(const node_pointer node) const noexcept(NO_EXCEPT) {
            return node != handler::nil;
        }

        inline void dispose(const node_pointer node) noexcept(NO_EXCEPT) {
            node_allocator_traits::destroy(this->_allocator, node);
            node_allocator_traits::deallocate(this->_allocator, node, 1);
        }
    };
};


} // namespace node_handlers

} // namespace lib
