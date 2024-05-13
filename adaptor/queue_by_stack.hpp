#pragma once


#include <concepts>
#include <utility>


#include "internal/dev_env.hpp"


namespace uni {


template<class In, class Out = In>
struct queue_by_stack {
    static_assert(std::same_as<typename In::value_type, typename Out::value_type>);
    static_assert(std::common_reference_with<typename In::size_type, typename Out::size_type>);

    using value_type = In::value_type;
    using size_type = std::common_type_t<typename In::size_type, typename Out::size_type>;

  protected:
    In _in;
    Out _out;

  private:
    inline void _shift() noexcept(NO_EXCEPT) {
        if(!this->_out.empty()) return;

        while(!this->_in.empty()) {
            this->_out.push(this->_in.top());
            this->_in.pop();
        }
    }

  public:
    queue_by_stack() noexcept = default;

    inline bool empty() const noexcept(NO_EXCEPT) {
        return this->_in.empty() && this->_out.empty();
    }

    inline auto size() const noexcept(NO_EXCEPT) {
        return this->_in.size() + this->_out.size();
    }


    inline decltype(auto) front() noexcept(NO_EXCEPT) {
        this->_shift();
        assert(!this->_out.empty());
        return this->_out.top();
    }

    template<std::convertible_to<value_type> T = value_type>
        requires std::is_move_constructible_v<T>
    inline decltype(auto) front_or(T&& val) const noexcept(NO_EXCEPT) {
        if(this->empty()) return static_cast<value_type>(std::forward<T>(val));
        else return this->front();
    }


    inline auto& clear() noexcept(NO_EXCEPT) {
        this->_in.clear(), this->_out.clear();
        return *this;
    }


    template<std::convertible_to<value_type> T = value_type>
    inline auto& push(T&& val) noexcept(NO_EXCEPT) {
        this->_in.push(std::forward<T>(val));
        return *this;
    }

    template<class... Args>
    inline decltype(auto) emplace(Args&&... args) noexcept(NO_EXCEPT) {
        return this->_in.emplace(std::forward<Args>(args)...);
    }


    auto& pop() noexcept(NO_EXCEPT) {
        this->_shift();
        assert(!this->_out.empty());

        this->_out.pop();
        return *this;
    }
};



} // namespace uni
