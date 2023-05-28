#pragma once

#include <unordered_map>
#include <map>
#include <memory>
#include <optional>

#include "internal/dev_env.hpp"

namespace lib {

template<class T, class ID = int, template<class,class> class storage = std::unordered_map>
struct restorable_stack {
  protected:
    struct node;
    using node_ptr = std::shared_ptr<node>;

    struct node {
        std::map<int,int> a;
        std::optional<T> val = std::nullopt;
        node_ptr parent;
    };

    node_ptr current;
    storage<ID,node_ptr> storage;

  public:
    restorable_stack() noexcept(NO_EXCEPT) { this->clear(); };

    inline bool empty() const noexcept(NO_EXCEPT) {
        return !current->val.has_value();
    }
    inline bool stored(ID x) const noexcept(NO_EXCEPT) {
        return storage.count(x);
    }

    inline T top() const noexcept(NO_EXCEPT) {
        return current->val.value();
    }

    template<class U>
    inline auto top_or(const U &&v) const noexcept(NO_EXCEPT) {
        return current->val.value_or(v);
    }

    inline void push(const T x) noexcept(NO_EXCEPT) {
        current.reset(new node{x, current});
    }
    inline void pop() noexcept(NO_EXCEPT) {
        current = current->parent;
    }
    inline void save(const ID x) noexcept(NO_EXCEPT) {
        storage[x] = current;
    }
    inline void load(const ID x) noexcept(NO_EXCEPT) {
        current = storage[x];
    }
    inline void clear() noexcept(NO_EXCEPT) {
        current.reset(new node{});
    }
};

} // namespace lib
