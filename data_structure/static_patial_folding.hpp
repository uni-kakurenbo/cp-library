#pragma once

// TODO

template<class T>
struct static_patial_folding {
    static_patial_folding() {}

    inline void set(size_type p, const S& x) noexcept(NO_EXCEPT) {
    }

    inline S get(size_type p) const noexcept(NO_EXCEPT) {
    }

    inline S fold(size_type l, size_type r) const noexcept(NO_EXCEPT) {
    }

    inline S fold_all() const noexcept(NO_EXCEPT) { return this->_values[1]; }

    inline void apply(size_type p, const F& f) noexcept(NO_EXCEPT) {
    }
    inline void apply(size_type l, size_type r, const F& f) noexcept(NO_EXCEPT) {
    }

};
