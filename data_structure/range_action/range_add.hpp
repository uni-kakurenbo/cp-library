#pragma once


#include <cassert>
#include <vector>

#include "internal/dev_env.hpp"

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "data_structure/fenwick_tree.hpp"
#include "data_structure/range_action/range_sum.hpp"


namespace lib {

namespace actions {


template<class T> struct range_add {};


} // namespace actions


template<class T>
struct fenwick_tree<actions::range_add<T>> : fenwick_tree<actions::range_sum<T>> {
  private:
    using core = fenwick_tree<actions::range_sum<T>>;

  public:
    using value_type = typename core::value_type;
    using size_type = typename core::size_type;

  protected:
    inline bool _validate_index_in_right_open([[maybe_unused]] const size_type p) const noexcept(NO_EXCEPT) {
        return 0 <= p and p < this->size();
    }
    inline bool _validate_index_in_closed([[maybe_unused]] const size_type p) const noexcept(NO_EXCEPT) {
        return 0 <= p and p <= this->size();
    }
    inline bool _validate_rigth_open_interval([[maybe_unused]] const size_type l, [[maybe_unused]] const size_type r) const noexcept(NO_EXCEPT) {
        return 0 <= l and l <= r and r <= this->size();
    }

    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    fenwick_tree() noexcept(NO_EXCEPT) : core() {}

    explicit fenwick_tree(const size_type size) noexcept(NO_EXCEPT) : core(size+1) {}

    fenwick_tree(const size_type size, const value_type& val) noexcept(NO_EXCEPT) : core(size+1, val) {}

    template<std::convertible_to<value_type> U>
    fenwick_tree(const std::initializer_list<U>& init_list) noexcept(NO_EXCEPT) : fenwick_tree(ALL(init_list)) {}

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    fenwick_tree(const I first, const S last) noexcept(NO_EXCEPT)
      : fenwick_tree(std::ranges::distance(first, last))
    {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->apply(pos, *itr);
    }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->core::size() - 1; }


    struct point_reference : internal::point_reference<fenwick_tree> {
        point_reference(fenwick_tree *const super, const size_type p) noexcept(NO_EXCEPT)
          : internal::point_reference<fenwick_tree>(super, super->_positivize_index(p))
        {
            this->_super->_validate_index_in_right_open(this->_pos);
        }

        operator value_type() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        value_type val() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }

        inline point_reference& set(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }

        inline point_reference& add(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator+=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator-=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, -v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<fenwick_tree> {
        range_reference(fenwick_tree *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<fenwick_tree>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            this->_super->_validate_rigth_open_interval(this->_begin, this->_end);
        }

        inline range_reference& apply(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator<<=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }

        inline range_reference& add(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator+=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
    };


    inline auto& add(const size_type first, const size_type last, const value_type& v) noexcept(NO_EXCEPT) {
        this->core::apply(first, v), this->core::apply(last, -v);
        return *this;
    }
    inline auto& add(const size_type p, const value_type& v) noexcept(NO_EXCEPT) { this->apply(p, p+1, v); return *this; }
    inline auto& add(const value_type& v) noexcept(NO_EXCEPT) { this->apply(0, this->size(), v); return *this; }

    inline auto& apply(const size_type first, const size_type last, const value_type& v) noexcept(NO_EXCEPT) {
        this->core::apply(first, v), this->core::apply(last, -v);
        return *this;
    }
    inline auto& apply(const size_type p, const value_type& v) noexcept(NO_EXCEPT) { this->apply(p, p+1, v); return *this; }
    inline auto& apply(const value_type& v) noexcept(NO_EXCEPT) { this->apply(0, this->size(), v); return *this; }

    inline auto& set(const size_type p, const value_type& v) noexcept(NO_EXCEPT) { this->add(p, v + -this->get(p)).val(); return *this; }

    inline value_type get(const size_type p) const noexcept(NO_EXCEPT) {
        assert(0 <= p and p < this->size());
        return this->core::fold(p+1);
    }
    inline point_reference operator[](const size_type p) noexcept(NO_EXCEPT) { return point_reference(this, p); }

    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type, fenwick_tree, iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(const fenwick_tree *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace lib
