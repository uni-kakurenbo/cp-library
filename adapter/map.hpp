#pragma once


#include <map>
#include <unordered_map>

#include "internal/types.hpp"
#include "internal/exception.hpp"

#include "adapter/set.hpp"


namespace lib {

namespace internal {


template<class Map>
using map_wrapper_base = set_wrapper<Map>;


template<class Map> struct map_wrapper : map_wrapper_base<Map> {
  private:
    using base = map_wrapper_base<Map>;

  public:
    using base::base;
    using mapped_type = typename base::mapped_type;
    using key_type = typename base::key_type;

  protected:
    bool _default_type = 0;
    mapped_type _default_val = {};
    mapped_type (*_default_func)(void);

    inline mapped_type _get_default() const noexcept(NO_EXCEPT) {
        return this->_default_type ? this->_default_func() : this->_default_val;
    }

  public:
    inline auto& set_default(const mapped_type& val) noexcept(NO_EXCEPT) {
        this->_default_val = val;
        this->_default_type = 0;
        return *this;
    }

    inline auto& set_default(mapped_type (*const func)(void)) noexcept(NO_EXCEPT) {
        this->_default_func = func;
        this->_default_type = 1;
        return *this;
    }

    inline auto& operator[](const key_type key) noexcept(NO_EXCEPT) {
        auto found = this->base::find(key);
        if(found == this->base::end()) return this->base::operator[](key) = this->_get_default();
        return found->second;
    }

    inline const auto& operator[](const key_type key) const noexcept(NO_EXCEPT) {
        const auto found = this->base::find(key);
        if(found == this->base::end()) return this->base::operator[](key) = this->_get_default();
        return found->second;
    }

    inline auto& operator()(const key_type key) noexcept(NO_EXCEPT) {
        return this->base::operator[](key);
    }

    inline const auto& operator()(const key_type key) const noexcept(NO_EXCEPT) {
        return this->base::operator[](key);
    }
};


} // namespace internal


template<class... Args> using map = internal::map_wrapper<std::map<Args...>>;
template<class... Args> using unordered_map = internal::map_wrapper<std::unordered_map<Args...>>;
template<class... Args> using multimap = internal::map_wrapper<std::multimap<Args...>>;
template<class... Args> using unordered_multimap = internal::map_wrapper<std::unordered_multimap<Args...>>;


} // namespace lib
