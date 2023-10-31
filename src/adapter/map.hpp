#pragma once


#include <map>
#include <unordered_map>
#include <functional>

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
    using default_func_noarg_type = std::function<mapped_type(void)>;
    using default_func_type = std::function<mapped_type(key_type)>;

    int _default_type = 0;
    mapped_type _default_val = {};
    default_func_noarg_type _default_func_noarg;
    default_func_type _default_func;

    inline mapped_type _get_default(const key_type& key) const noexcept(NO_EXCEPT) {
        if(this->_default_type == 0) return this->_default_val;
        if(this->_default_type == 1) return this->_default_func_noarg();
        if(this->_default_type == 2) return this->_default_func(key);
        else assert(false);
    }

  public:
    inline auto& set_default(const mapped_type& val) noexcept(NO_EXCEPT) {
        this->_default_val = val;
        this->_default_type = 0;
        return *this;
    }

    inline auto& set_default(const default_func_noarg_type func) noexcept(NO_EXCEPT) {
        this->_default_func_noarg = func;
        this->_default_type = 1;
        return *this;
    }

    inline auto& set_default(const default_func_type func) noexcept(NO_EXCEPT) {
        this->_default_func = func;
        this->_default_type = 2;
        return *this;
    }

    inline auto& operator[](const key_type& key) noexcept(NO_EXCEPT) {
        auto found = this->base::find(key);
        if(found == this->base::end()) return this->base::emplace(key, this->_get_default(key)).first->second;
        return found->second;
    }

    inline auto& operator()(const key_type& key) noexcept(NO_EXCEPT) {
        return this->base::operator[](key);
    }

    inline std::optional<mapped_type> get(const key_type& key) const noexcept(NO_EXCEPT) {
        const auto found = this->base::find(key);
        if(found == this->base::end()) return {};
        return found->second;
    }
};


} // namespace internal


template<class... Args> using map = internal::map_wrapper<std::map<Args...>>;
template<class... Args> using unordered_map = internal::map_wrapper<std::unordered_map<Args...>>;
template<class... Args> using multimap = internal::map_wrapper<std::multimap<Args...>>;
template<class... Args> using unordered_multimap = internal::map_wrapper<std::unordered_multimap<Args...>>;


} // namespace lib
