#pragma once

#include <unordered_map>
#include <map>
#include <memory>
#include <optional>

#include "internal/dev_env.hpp"

namespace lib {

template<class T, class ID = int, template<class,class> class storage = std::unordered_map>
struct restorable_stack {
    using value_type = T;
    using key_type = ID;

  protected:
    struct node;
    using node_ptr = std::shared_ptr<node>;

    struct node {
        std::optional<value_type> val = std::nullopt;
        node_ptr parent;
    };

    node_ptr _current;
    storage<key_type,node_ptr> _storage;

  public:
    restorable_stack() noexcept(NO_EXCEPT) { this->clear(); };

    inline bool empty() const noexcept(NO_EXCEPT) {
        return !this->_current->val.has_value();
    }
    inline bool stored(key_type x) const noexcept(NO_EXCEPT) {
        return this->_storage.count(x);
    }

    inline const value_type& top() const noexcept(NO_EXCEPT) {
        return this->_current->val.value();
    }

    template<class U>
    inline auto top_or(const U &&v) const noexcept(NO_EXCEPT) {
        return this->_current->val.value_or(v);
    }

    inline auto& push(const value_type& x) noexcept(NO_EXCEPT) {
        this->_current.reset(new node{ x, this->_current });
        return *this;
    }
    inline auto& pop() noexcept(NO_EXCEPT) {
        this->_current = this->_current->parent;
        return *this;
    }
    inline auto& save(const key_type x) noexcept(NO_EXCEPT) {
        this->_storage[x] = this->_current;
        return *this;
    }
    inline auto& load(const key_type x) noexcept(NO_EXCEPT) {
        this->_current = this->_storage[x];
        return *this;
    }
    inline auto& clear() noexcept(NO_EXCEPT) {
        this->_current.reset(new node{});
        return *this;
    }
};

} // namespace lib
