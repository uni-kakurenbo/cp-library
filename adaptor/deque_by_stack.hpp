#pragma once


#include <concepts>
#include <utility>
#include <vector>
#include <ranges>
#include <algorithm>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"


namespace uni {


template<class Front, class Back = Front>
struct deque_by_stack {
    static_assert(std::same_as<typename Front::value_type, typename Back::value_type>);
    static_assert(std::common_reference_with<typename Front::size_type, typename Back::size_type>);

    using value_type = Front::value_type;
    using size_type = std::common_type_t<typename Front::size_type, typename Back::size_type>;

  protected:
    Front _front;
    Back _back;

  private:
    template<std::ptrdiff_t OFFSET>
    inline void _distribute() noexcept(NO_EXCEPT) {
        if(this->empty()) return;

        std::vector<value_type> temp;
        temp.reserve(this->size());

        if(this->_front.empty()) {
            while(!this->_back.empty()) {
                temp.push_back(this->_back.top());
                this->_back.pop();
            }
            std::ranges::reverse(temp);
        }
        else if(this->_back.empty()) {
            while(!this->_front.empty()) {
                temp.push_back(this->_front.top());
                this->_front.pop();
            }
        }
        else {
            return;
        }

        assert(this->empty());

        const auto size = std::ranges::ssize(temp);
        const auto mid = (size + OFFSET) / 2;

        REPD(i, mid) this->_front.push(temp[i]);
        REP(i, mid, size) this->_back.push(temp[i]);
    }

  public:
    deque_by_stack() noexcept = default;

    inline bool empty() const noexcept(NO_EXCEPT) {
        return this->_front.empty() && this->_back.empty();
    }

    inline auto size() const noexcept(NO_EXCEPT) {
        return this->_front.size() + this->_back.size();
    }


    inline decltype(auto) front() const noexcept(NO_EXCEPT) {
        this->_distribute<1>();
        assert(!this->_front.empty());
        return this->_front.top();
    }

    inline decltype(auto) back() const noexcept(NO_EXCEPT) {
        this->_distribute<0>();
        assert(!this->_back.empty());
        return this->_back.top();
    }


    template<std::convertible_to<value_type> T = value_type>
        requires std::is_move_constructible_v<T>
    inline decltype(auto) front_or(T&& val) const noexcept(NO_EXCEPT) {
        if(this->empty()) return static_cast<value_type>(std::forward<T>(val));
        else return this->front();
    }

    template<std::convertible_to<value_type> T = value_type>
        requires std::is_move_constructible_v<T>
    inline decltype(auto) back_or(T&& val) const noexcept(NO_EXCEPT) {
        if(this->empty()) return static_cast<value_type>(std::forward<T>(val));
        else return this->back();
    }


    inline auto& clear() noexcept(NO_EXCEPT) {
        this->_front.clear(), this->_back.clear();
        return *this;
    }


    template<std::convertible_to<value_type> T = value_type>
    inline auto& push_front(T&& val) noexcept(NO_EXCEPT) {
        this->_front.push(std::forward<T>(val));
        return *this;
    }

    template<std::convertible_to<value_type> T = value_type>
    inline auto& push_back(T&& val) noexcept(NO_EXCEPT) {
        this->_back.push(std::forward<T>(val));
        return *this;
    }


    template<class... Args>
    inline decltype(auto) emplace_front(Args&&... args) noexcept(NO_EXCEPT) {
        return this->_front.emplace(std::forward<Args>(args)...);
    }

    template<class... Args>
    inline decltype(auto) emplace_back(Args&&... args) noexcept(NO_EXCEPT) {
        return this->_back.emplace(std::forward<Args>(args)...);
    }


    auto& pop_front() noexcept(NO_EXCEPT) {
        this->_distribute<1>();
        assert(!this->_front.empty());

        this->_front.pop();
        return *this;
    }

    auto& pop_back() noexcept(NO_EXCEPT) {
        this->_distribute<0>();
        assert(!this->_back.empty());

        this->_back.pop();
        return *this;
    }
};



} // namespace uni
