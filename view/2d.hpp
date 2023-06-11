#pragma once


#include <cassert>
#include <functional>
#include <cstring>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "view/internal/base.hpp"


namespace lib {


template<class Ref> struct view_2d : internal::view_impl::base {
    using size_type = internal::size_t;

    using transposer = std::function<std::pair<size_type,size_type>(const size_type, const size_type)>;

  protected:
    transposer _transposer = [&](const size_type i, const size_type j) { return { i, j }; };

    Ref* _ref;
    size_type _pos0, _pos1;

  public:
    view_2d(Ref *const ref) noexcept(NO_EXCEPT) : _ref(ref) {}

    inline view_2d& reset(Ref *const ref) { this->_ref = ref; return *this; }

    inline Ref* operator->() const noexcept(NO_EXCEPT) { return this->_ref; }
    inline Ref& operator*() const noexcept(NO_EXCEPT) { return *this->_ref; }

    inline size_type size() const noexcept(NO_EXCEPT) { return std::size(*this->_ref); }
    inline size_type height() const noexcept(NO_EXCEPT) { return std::size(*this->_ref); }
    inline size_type width() const noexcept(NO_EXCEPT) { return std::size(*this->_ref->begin()); }

    inline view_2d& drop(const std::initializer_list<size_type> pos) noexcept(NO_EXCEPT) {
        assert(pos.size() == std::size(this->_pos));
        for(auto a=pos.begin(),b=std::begin(this->_pos); a != pos.end(); ++a, ++b) *b = *a;
        return *this;
    }
    template<class... Pos> inline view_2d& drop(const Pos... pos) noexcept(NO_EXCEPT) { return this->drop({ pos... }); }
    inline view_2d& drop() noexcept(NO_EXCEPT) { std::memset(this->_pos, 0, sizeof(this->_pos)); return *this; }

    inline const view_2d& transpose(const transposer& f) noexcept(NO_EXCEPT) { this->_transposer = f; return *this; }

    inline auto& operator()(const size_type i, const size_type j) noexcept(NO_EXCEPT) { return this->operator[]({ i, j }); }
    inline const auto& operator()(const size_type i, const size_type j) const noexcept(NO_EXCEPT) { return this->operator[]({ i, j }); }

    inline auto& operator[](std::pair<size_type,size_type> pos) noexcept(NO_EXCEPT) {
        pos = this->_transposer(pos);
        return this->_ref->operator[](pos.first)[pos.second];
    }
    inline const auto& operator[](std::pair<size_type,size_type> pos) const noexcept(NO_EXCEPT) {
        pos = this->_transposer(pos);
        return this->_ref->operator[](pos.first)[pos.second];
    }
};


} // namespace lib
