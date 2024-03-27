#pragma once


#include <memory>

#include "internal/dev_env.hpp"


namespace lib {

namespace node_handlers {


template<class Allocator>
struct copyable {
    template<class NodeType>
    struct handler {
        using allocator_type = Allocator;

        using node_type = NodeType;
        using node_pointer = std::shared_ptr<NodeType>;

      private:
        using allocator_traits = std::allocator_traits<allocator_type>;

        using node_allocator_type = allocator_traits::template rebind_alloc<node_type>;
        using node_allocator_traits = std::allocator_traits<node_allocator_type>;

        [[no_unique_address]] node_allocator_type _allocator;

      public:
        inline static node_pointer nil = std::make_shared<node_type>();


        handler(const allocator_type& allocator) noexcept(NO_EXCEPT)
          : _allocator(allocator)
        {}

        handler(const handler& source) noexcept(NO_EXCEPT)
          : _allocator(node_allocator_traits::select_on_container_copy_construction(source._allocator))
        {}

        handler(handler&& source) noexcept(NO_EXCEPT)
          : _allocator(node_allocator_traits::select_on_container_copy_construction(source._allocator))
        {}


        template<class... Args>
        inline node_pointer create(Args&&... args) noexcept(NO_EXCEPT) {
            return std::allocate_shared<node_type>(this->_allocator, std::forward<Args>(args)...);
        }

        inline node_pointer clone(const node_pointer& ptr) noexcept(NO_EXCEPT) {
            return this->create(*ptr);
        }

        inline void dispose(const node_pointer&) const noexcept {}
    };
};


template<class Allocator>
struct reusing {
    template<class NodeType>
    struct handler {
        using allocator_type = Allocator;

        using node_type = NodeType;
        using node_pointer = std::add_pointer_t<NodeType>;

      private:
        using allocator_traits = std::allocator_traits<allocator_type>;

        using node_allocator_type = allocator_traits::template rebind_alloc<node_type>;
        using node_allocator_traits = std::allocator_traits<node_allocator_type>;

        [[no_unique_address]] node_allocator_type _allocator;

        inline static int _instance_count = 0;

      public:
        inline static node_pointer nil;


        handler(const allocator_type& allocator) noexcept(NO_EXCEPT) : _allocator(allocator) {
            if(handler::_instance_count++ == 0) {
                handler::nil = new node_type{};
            }
        }

        ~handler() noexcept {
            if(--handler::_instance_count == 0) {
                delete handler::nil;
            }
        }


        handler(const handler& source) noexcept(NO_EXCEPT)
          : _allocator(node_allocator_traits::select_on_container_copy_construction(source._allocator))
        {}

        handler(handler&& source) noexcept(NO_EXCEPT)
          : _allocator(node_allocator_traits::select_on_container_copy_construction(source._allocator))
        {}


        template<class... Args>
        inline node_pointer create(Args&&... args) noexcept(NO_EXCEPT) {
            node_pointer node = node_allocator_traits::allocate(this->_allocator, 1);
            node_allocator_traits::construct(this->_allocator, node, std::forward<Args>(args)...);

            return node;
        }

        inline node_pointer clone(const node_pointer ptr) const noexcept { return ptr; }

        inline void dispose(const node_pointer node) noexcept(NO_EXCEPT) {
            if(node == node_type::nil) return;

            this->dispose(node->left);
            this->dispose(node->right);

            node_allocator_traits::deallocate(this->_allocator, node, 1);
        }
    };
};


} // namespace node_handlers

} // namespace lib
