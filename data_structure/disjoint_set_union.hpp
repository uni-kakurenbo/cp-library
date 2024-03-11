#pragma once


#include <cassert>
#include <algorithm>
#include <cassert>
#include <vector>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/vector.hpp"


namespace lib {

//Thanks to: atcoder::dsu
struct dsu {
    using size_type = internal::size_t;

  private:
    size_type _n, _group_count;
    // root node: -1 * component size
    // otherwise: parent
    std::vector<size_type> _parent_or_size;

  public:
    dsu() noexcept(NO_EXCEPT) : _n(0) {}
    explicit dsu(const size_type n) noexcept(NO_EXCEPT) : _n(n), _group_count(n), _parent_or_size(n, -1) {}

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type group_count() const noexcept(NO_EXCEPT) { return this->_group_count; }

    inline size_type merge(const size_type a, const size_type b) noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);

        size_type x = this->leader(a), y = this->leader(b);
        if (x == y) return x;

        --this->_group_count;

        if (-this->_parent_or_size[x] < -this->_parent_or_size[y]) std::swap(x, y);

        this->_parent_or_size[x] += this->_parent_or_size[y];
        this->_parent_or_size[y] = x;

        return x;
    }

    inline bool same(const size_type a, const size_type b) noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return this->leader(a) == this->leader(b);
    }

    inline size_type leader(const size_type a) noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        if (this->_parent_or_size[a] < 0) return a;
        return this->_parent_or_size[a] = this->leader(this->_parent_or_size[a]);
    }

    inline size_type size(const size_type a) noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        return -this->_parent_or_size[this->leader(a)];
    }

    inline vector<vector<size_type>> groups() noexcept(NO_EXCEPT) {
        vector<size_type> leader_buf(_n), group_size(_n);
        for (size_type i = 0; i < _n; i++) {
            leader_buf[i] = this->leader(i);
            group_size[leader_buf[i]]++;
        }

        vector<vector<size_type>> result(_n);
        for (size_type i = 0; i < _n; i++) {
            result[i].reserve(group_size[i]);
        }

        for (size_type i = 0; i < _n; i++) {
            result[leader_buf[i]].push_back(i);
        }

        result.erase(
            remove_if(result.begin(), result.end(), [&](const auto& v) { return v.empty(); }),
            result.end()
        );

        return result;
    }
};


} // namespace lib
