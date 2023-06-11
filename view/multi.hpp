#pragma once


#include <cassert>
#include <functional>
#include <cstring>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "view/internal/base.hpp"


namespace lib {



template<class Ref, internal::size_t D>
struct multi_view : internal::view_impl::base {
    using size_type = internal::size_t;

    using transposer = std::function<std::vector<size_type>(const std::initializer_list<size_type>&)>;

  protected:
    transposer _transposer = [&](const std::initializer_list<size_type>& pos) { return std::vector<size_type>(pos.begin(), pos.end()); };

    Ref* _ref;
    size_type _pos[D] = {};

    template<class R, class I, size_type d = 0> inline auto& _access(
        R *const r,
        const I p
    ) noexcept(NO_EXCEPT) {
        if constexpr(d == D) {
            return *r;
        }
        else {
            size_type index = this->_pos[d] + *std::next(p, d);

            assert(0 <= index && index < static_cast<size_type>(std::size(*r)));

            using nR = std::remove_reference_t<decltype(r->operator[](0))>;
            return multi_view::_access<nR,I,d+1>(&r->operator[](index), p);
        }
    }

  public:
    multi_view(Ref *const ref) noexcept(NO_EXCEPT) : _ref(ref) {}

    inline multi_view& reset(Ref *const ref) noexcept(NO_EXCEPT) { this->_ref = ref; return *this; }

    inline Ref* operator->() const noexcept(NO_EXCEPT) { return this->_ref; }
    inline Ref& operator*() const noexcept(NO_EXCEPT) { return *this->_ref; }

    inline size_type size() const noexcept(NO_EXCEPT) { return std::size(*this->_ref); }

    inline multi_view& drop(const std::initializer_list<size_type> pos) noexcept(NO_EXCEPT) {
        assert(pos.size() == std::size(this->_pos));
        for(auto a=pos.begin(),b=std::begin(this->_pos); a != pos.end(); ++a, ++b) *b = *a;
        return *this;
    }
    template<class... Pos> inline multi_view& drop(const Pos... pos) noexcept(NO_EXCEPT) { return this->drop({ pos... }); }
    inline multi_view& drop() noexcept(NO_EXCEPT) { std::memset(this->_pos, 0, sizeof(this->_pos)); return *this; }

    inline const multi_view& transpose(const transposer& f) noexcept(NO_EXCEPT) { this->_transposer = f; return *this; }


    inline auto& operator[](const std::initializer_list<size_type> pos_) noexcept(NO_EXCEPT) {
        const auto pos = this->_transposer(pos_);
        return this->_access(this->_ref, pos.begin());
    }
    inline const auto& operator[](std::initializer_list<size_type> pos_) const noexcept(NO_EXCEPT) {
        const auto pos = this->_transposer(pos_);
        return this->_access(this->_ref, pos.begin());
    }

    template<class... Pos> inline auto& operator()(const Pos... pos) noexcept(NO_EXCEPT) { return this->operator[]({ pos... }); }
    template<class... Pos> inline const auto& operator()(const Pos... pos) const noexcept(NO_EXCEPT) { return this->operator[]({ pos... }); }
};

} // namespace lib
