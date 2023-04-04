#pragma once


#include <vector>

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "data_structure/range_action/range_sum.hpp"
#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace actions {

template<class T> struct range_add {
    static constexpr flags tags{ flags::range_operation };
};

} // namespace actions


template<class T> struct fenwick_tree<actions::range_add<T>> : fenwick_tree<actions::range_sum<T>> {
  private:
    using core = fenwick_tree<actions::range_sum<T>>;

  public:
    using value_type = typename core::value_type;
    using size_type = typename core::size_type;

  protected:
    inline bool _validate_index_in_right_open([[maybe_unused]] const size_type p) const {
        return 0 <= p and p < this->size();
    }
    inline bool _validate_index_in_closed([[maybe_unused]] const size_type p) const {
        return 0 <= p and p <= this->size();
    }
    inline bool _validate_rigth_open_interval([[maybe_unused]] const size_type l, [[maybe_unused]] const size_type r) const {
        return 0 <= l and l <= r and r <= this->size();
    }

    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    explicit fenwick_tree(const size_type size = 0) : core(size+1) {}
    explicit fenwick_tree(const size_type size, const value_type& val) : core(size+1, val) {}

    template<class U> fenwick_tree(const std::initializer_list<U>& init_list) : fenwick_tree(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    fenwick_tree(const I first, const I last) : fenwick_tree(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->apply(pos, *itr);
    }

    inline size_type size() const { return this->core::size() - 1; }


    struct point_reference : internal::point_reference<fenwick_tree> {
        point_reference(fenwick_tree *const super, const size_type p)
          : internal::point_reference<fenwick_tree>(super, super->_positivize_index(p))
        {
            this->_super->_validate_index_in_right_open(this->_pos);
        }

        operator value_type() const { return this->_super->get(this->_pos); }
        value_type val() const { return this->_super->get(this->_pos); }

        inline point_reference& set(const value_type& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const value_type& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const value_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const value_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }

        inline point_reference& add(const value_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator+=(const value_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator-=(const value_type& v) {
            this->_super->apply(this->_pos, -v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<fenwick_tree> {
        range_reference(fenwick_tree *const super, const size_type l, const size_type r)
          : internal::range_reference<fenwick_tree>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            this->_super->_validate_rigth_open_interval(this->_begin, this->_end);
        }

        inline range_reference& apply(const value_type& v) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator<<=(const value_type& v) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }

        inline range_reference& add(const value_type& v) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator+=(const value_type& v) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator-=(const value_type& v) {
            this->_super->apply(this->_begin, this->_end, -v);
            return *this;
        }
    };


    inline auto& add(const size_type first, const size_type last, const value_type& v) {
        this->core::apply(first, v), this->core::apply(last, -v);
        return *this;
    }
    inline auto& add(const size_type p, const value_type& v) { this->apply(p, p+1, v); return *this; }
    inline auto& add(const value_type& v) { this->apply(0, this->size(), v); return *this; }
    inline auto& operator+=(const value_type& v) { this->apply(0, this->size(), v); return *this; }
    inline auto& operator-=(const value_type& v) { this->apply(0, this->size(), -v); return *this; }

    inline auto& apply(const size_type first, const size_type last, const value_type& v) {
        this->core::apply(first, v), this->core::apply(last, -v);
        return *this;
    }
    inline auto& apply(const size_type p, const value_type& v) { this->apply(p, p+1, v); return *this; }
    inline auto& apply(const value_type& v) { this->apply(0, this->size(), v); return *this; }

    inline auto& set(const size_type p, const value_type& v) { this->add(p, v + -this->get(p)).val(); return *this; }

    inline value_type get(const size_type p) const {
        assert(0 <= p and p < this->size());
        return this->core::fold(p+1);
    }
    inline point_reference operator[](const size_type p) { return point_reference(this, p); }

    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,fenwick_tree>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const fenwick_tree *const ref, const size_type pos) : iterator_interface(ref, pos) {}

        inline value_type operator*() const { return this->ref()->get(this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const { return *(*this + count); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
