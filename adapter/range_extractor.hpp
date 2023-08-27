#pragma once


#include "internal/dev_env.hpp"


namespace lib {


template<class Container>
struct range_extractor : Container {
    using size_type = typename Container::size_type;
    using value_type = typename Container::value_type;

  protected:
    size_type _begin = 0;
    size_type _end;

    int _default_type = 0;
    value_type _default_val = {};
    value_type (*_default_func_noarg)(void);
    value_type (*_default_func)(size_type);

    inline value_type _get_default(const size_type key) const noexcept(NO_EXCEPT) {
        if(this->_default_type == 0) return this->_default_val;
        if(this->_default_type == 1) return this->_default_func_noarg();
        if(this->_default_type == 2) return this->_default_func(key);
        else assert(false);
    }

  public:
    template<class... Args>
    explicit range_extractor(const Args&... args) noexcept(NO_EXCEPT) : Container(args...) {
        this->_begin = 0;
        this->_end = this->size();
    }

    inline auto& extract(const size_type begin, const size_type end) noexcept(NO_EXCEPT) {
        assert(begin <= end);
        this->_begin = begin, this->_end = end;
        return *this;
    }


    inline auto& set_default(const value_type& val) noexcept(NO_EXCEPT) {
        this->_default_val = val;
        this->_default_type = 0;
        return *this;
    }

    inline auto& set_default(value_type (*const func)(void)) noexcept(NO_EXCEPT) {
        this->_default_func_noarg = func;
        this->_default_type = 1;
        return *this;
    }

    inline auto& set_default(value_type (*const func)(size_type)) noexcept(NO_EXCEPT) {
        this->_default_func = func;
        this->_default_type = 2;
        return *this;
    }


    inline auto& operator[](const size_type pos) noexcept(NO_EXCEPT) {
        if(pos < this->_begin or this->_end <= pos) return range_extractor::_tmp = this->_get_default();
        return this->Container::operator[](pos);
    }

    inline const auto& operator[](const size_type pos) const noexcept(NO_EXCEPT) {
        if(pos < this->_begin or this->_end <= pos) return this->_get_default();
        return this->Container::operator[](pos);
    }

    inline auto& operator()(const size_type pos) noexcept(NO_EXCEPT) {
        return this->Container::operator[](pos);
    }

    inline const auto& operator()(const size_type pos) const noexcept(NO_EXCEPT) {
        return this->Container::operator[](pos);
    }
};


}
