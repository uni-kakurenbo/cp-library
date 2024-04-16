#pragma once


#include <cassert>
#include <vector>
#include <functional>
#include <queue>
#include <optional>
#include <concepts>
#include <ranges>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace uni {


// Thanks to: https://qiita.com/drken/items/1b7e6e459c24a83bb7fd
template<
    class T,
    std::ranges::output_range<T> Container = std::vector<T>,
    std::strict_weak_order<T, T> Comparer = std::less<T>,
    std::strict_weak_order<T, T> RevComparer = std::greater<T>,
    std::integral Size = internal::size_t
>
struct kth_element {
    using value_type = T;
    using size_type = Size;

  protected:
    size_type _k;
    std::priority_queue<T, Container, Comparer> _small;
    std::priority_queue<T, Container, RevComparer> _large;

  public:
    kth_element(const size_type k = 0) noexcept(NO_EXCEPT) : _k(k + 1) { assert(k >= 0); }

    inline bool has() const noexcept(NO_EXCEPT) { return std::ssize(this->_small) == this->_k; }

    inline value_type value() const noexcept(NO_EXCEPT) { return this->_small.top(); }

    inline std::optional<value_type> get() const noexcept(NO_EXCEPT) {
        if(this->has()) return this->value();
        return {};
    }

    template<std::convertible_to<T> U>
    inline auto value_or(U&& v) const noexcept(NO_EXCEPT) {
        return this->get().value_or(std::forward<U>(v));
    }

    inline void push(const value_type& v) noexcept(NO_EXCEPT) {
        if(std::ssize(this->_small) < this->_k) {
            this->_small.push(v);
            return;
        }

        const value_type kth = this->_small.top();

        if(Comparer{}(v, kth)) {
            this->_small.pop();
            this->_small.push(v);
            this->_large.push(kth);
        }
        else {
            this->_large.push(v);
        }
    }

    inline void pop() noexcept(NO_EXCEPT) {
        assert(this->has());

        this->_small.pop();

        if(this->_large.empty()) return;

        const value_type v = this->_large.top();

        this->_large.pop();
        this->_small.push(v);
    }
};


} // namespace uni
