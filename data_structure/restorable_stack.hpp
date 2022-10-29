#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

template<class T, class ID = int, template<class,class> class Storage = std::unordered_map>
struct RestorableStack {
  protected:
    struct Node;
    using NodePtr = std::shared_ptr<Node>;

    struct Node {
        std::optional<T> val = std::nullopt;
        NodePtr parent;
    };

    NodePtr node;
    Storage<ID,NodePtr> storage;

  public:
    RestorableStack() { this->clear(); };

    inline bool empty() const {
        return !node->val.has_value();
    }
    inline bool stored(ID x) const {
        return storage.count(x);
    }

    inline T top() const {
        return node->val.value();
    }

    template<class U>
    inline auto top_or(const U &&v) const {
        return node->val.value_or(v);
    }

    inline void push(const T x) {
        node.reset(new Node{x, node});
    }
    inline void pop() {
        node = node->parent;
    }
    inline void save(const ID x) {
        storage[x] = node;
    }
    inline void load(const ID x) {
        node = storage[x];
    }
    inline void clear() {
        node.reset(new Node{});
    }
};
