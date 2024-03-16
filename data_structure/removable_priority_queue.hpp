#pragma once


#include <type_traits>
#include <cassert>
#include <queue>
#include <set>


#include "internal/dummy.hpp"
#include "internal/dev_env.hpp"

namespace lib {


namespace internal {


template<class T, class... Ts>
concept can_removable_priority_queue =
    sizeof...(Ts) == 0 &&
    requires (T pq, typename T::value_type v) {
        { pq.size() } -> std::same_as<typename T::size_type>;
        { pq.empty() } -> std::same_as<bool>;

        pq.top();
        pq.pop();

        pq.push(v);
        pq.emplace(v);
    };

} // namespace internal


template<class... Ts>
struct removable_priority_queue {};

template<class ValueType, class... Args>
    requires
        (!internal::can_removable_priority_queue<ValueType, Args...>) &&
        requires () {
            typename std::priority_queue<ValueType, Args...>;
        }
struct removable_priority_queue<ValueType, Args...> : removable_priority_queue<std::priority_queue<ValueType, Args...>> {
    using removable_priority_queue<std::priority_queue<ValueType, Args...>>::removable_priority_queue;
};

template<internal::can_removable_priority_queue PriorityQueue>
struct removable_priority_queue<PriorityQueue>
  : removable_priority_queue<PriorityQueue, internal::dummy>
{
    using removable_priority_queue<PriorityQueue, internal::dummy>::removable_priority_queue;
};

template<
    internal::can_removable_priority_queue PriorityQueue,
    class Multiset
>
struct removable_priority_queue<PriorityQueue, Multiset> : PriorityQueue {
    using base_type = PriorityQueue;
    using multiset_type = Multiset;

    using size_type = typename PriorityQueue::size_type;
    using value_type = typename PriorityQueue::value_type;
    using const_reference = typename PriorityQueue::const_reference;

  private:
    using self = removable_priority_queue<PriorityQueue, Multiset>;

    base_type _deleted;
    multiset_type _elements;

    static constexpr bool CHECK_EXISTANCE = !std::same_as<multiset_type, internal::dummy>;

    void _delete() noexcept(NO_EXCEPT) {
        while(!this->_deleted.empty() && this->_deleted.top() == this->base_type::top()) {
            this->base_type::pop();
            this->_deleted.pop();
        }
    }

  public:
    using base_type::base_type;


    size_type size() noexcept(NO_EXCEPT) {
        if constexpr(!self::CHECK_EXISTANCE) {
            assert(this->base_type::size() >= this->_deleted.size());
        }
        return this->base_type::size() - this->_deleted.size();
    }

    size_type empty() noexcept(NO_EXCEPT) {
        if constexpr(!self::CHECK_EXISTANCE) {
            assert(this->base_type::size() >= this->_deleted.size());
        }
        return this->base_type::size() == this->_deleted.size();
    }


    void push(const value_type& v) noexcept(NO_EXCEPT) {
        if constexpr(self::CHECK_EXISTANCE) this->_elements.insert(v);
        return this->base_type::push(v);
    }

    void push(value_type&& v) noexcept(NO_EXCEPT) {
        if constexpr(self::CHECK_EXISTANCE) this->_elements.insert(std::move(v));
        return this->base_type::push(std::move(v));
    }

    template<class... Args>
    void emplace(Args&&... args) noexcept(NO_EXCEPT) {
        if constexpr(self::CHECK_EXISTANCE) this->_elements.emplace(args...);
        return this->base_type::emplace(std::forward<Args>(args)...);
    }


    std::conditional_t<self::CHECK_EXISTANCE, bool, void> remove(const value_type& v) noexcept(NO_EXCEPT) {
        if constexpr(self::CHECK_EXISTANCE) {
            if(!this->_elements.contains(v)) return false;
        }

        this->_deleted.push(v);

        if constexpr(self::CHECK_EXISTANCE) return true;
    }

    std::conditional_t<self::CHECK_EXISTANCE, bool, void> remove(value_type&& v) noexcept(NO_EXCEPT) {
        if constexpr(self::CHECK_EXISTANCE) {
            if(!this->_elements.contains(v)) return false;
        }

        this->_deleted.push(std::move(v));

        if constexpr(self::CHECK_EXISTANCE) return true;
    }

    template<class... Args>
    std::conditional_t<self::CHECK_EXISTANCE, bool, void> eliminate(Args&&... args) noexcept(NO_EXCEPT) {
        if constexpr(self::CHECK_EXISTANCE) {
            if(!this->_elements.contains({ args... })) return false;
        }

        this->_deleted.emplace(std::forward<Args>(args)...);

        if constexpr(self::CHECK_EXISTANCE) return true;
    }


    const_reference top() noexcept(NO_EXCEPT) {
        this->_delete();
        return this->base_type::top();
    }


    void pop() noexcept(NO_EXCEPT) {
        this->_delete();
        this->base_type::pop();
    }
};


} // namespace lib
