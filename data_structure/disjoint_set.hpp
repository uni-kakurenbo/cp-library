#pragma once


#include <cassert>
#include <algorithm>
#include <cassert>
#include <vector>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adaptor/vector.hpp"


namespace uni {

//Thanks to: atcoder::disjoint_set
struct disjoint_set {
    using size_type = internal::size_t;

  private:
    size_type _n = 0, _group_count = 0;
    // root node: -1 * component size
    // otherwise: parent
    std::vector<size_type> _parent_or_size;

  public:
    disjoint_set() noexcept = default;
    explicit disjoint_set(const size_type n) noexcept(NO_EXCEPT) : _n(n), _group_count(n), _parent_or_size(n, -1) {}

    inline auto size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline auto group_count() const noexcept(NO_EXCEPT) { return this->_group_count; }

    inline auto merge(const size_type a, const size_type b) noexcept(NO_EXCEPT) {
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
        if(this->_parent_or_size[a] < 0) return a;
        return this->_parent_or_size[a] = this->leader(this->_parent_or_size[a]);
    }

    inline auto size(const size_type a) noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        return -this->_parent_or_size[this->leader(a)];
    }

    inline auto groups() noexcept(NO_EXCEPT) {
        vector<size_type> leader_buf(_n), group_size(_n);

        REP(i, this->_n) {
            leader_buf[i] = this->leader(i);
            group_size[leader_buf[i]]++;
        }

        vector<vector<size_type>> result(_n);
        REP(i, this->_n) result[i].reserve(group_size[i]);

        REP(i, this->_n) result[leader_buf[i]].push_back(i);

        {
            const auto range = std::ranges::remove_if(result, [&](const auto& v) { return v.empty(); });
            result.erase(ALL(range));
        }

        return result;
    }
};


} // namespace uni
