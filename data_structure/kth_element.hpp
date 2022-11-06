#pragma once

#include <vector>
#include <functional>
#include <queue>

#include "internal/types.hpp"

namespace Lib {

template<class T, class Container = std::vector<T>, class CompA = std::less<T>, class CompB = std::greater<T>>
struct KthElement {
  protected:
    Internal::Size _k;
    std::priority_queue<T,Container,CompA> small;
    std::priority_queue<T,Container,CompB> large;

  public:
    KthElement(Internal::Size k = 0) : _k(k) {}

    inline T get() const { return small.top(); }
    inline bool has() const { return small.size() == _k; }

    inline void push(T v) {
        if(small.size() < _k) {
            small.push(v);
            return;
        }
        T kth = small.top();
        if(v < kth) {
            small.pop(); small.push(v);
            large.push(kth);
        }
        else {
            large.push(v);
        }
    }

    inline void pop() {
        small.pop();
        if(large.empty()) return;
        T v = large.top(); large.pop();
        small.push(v);
    }
};

} // namespace Lib
