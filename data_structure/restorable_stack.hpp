#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

namespace lib {

template<class T, class ID = int, template<class,class> class storage = std::unordered_map>
struct restorable_stack {
  protected:
    struct node;
    using node_ptr = std::shared_ptr<node>;

    struct node {
        std::optional<T> val = std::nullopt;
        node_ptr parent;
    };

    node_ptr current;
    storage<ID,node_ptr> storage;

  public:
    restorable_stack() { this->clear(); };

    inline bool empty() const {
        return !current->val.has_value();
    }
    inline bool stored(ID x) const {
        return storage.count(x);
    }

    inline T top() const {
        return current->val.value();
    }

    template<class U>
    inline auto top_or(const U &&v) const {
        return current->val.value_or(v);
    }

    inline void push(const T x) {
        current.reset(new node{x, current});
    }
    inline void pop() {
        current = current->parent;
    }
    inline void save(const ID x) {
        storage[x] = current;
    }
    inline void load(const ID x) {
        current = storage[x];
    }
    inline void clear() {
        current.reset(new node{});
    }
};

} // namespace lib
