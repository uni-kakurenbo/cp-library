#pragma once


#include "internal/iterator.hpp"


namespace lib {


template<class I>
struct joined_iterator {
    using iterator_type = I;

    iterator_type _f0, _l0;
    iterator_type _f1, _l1;

    explicit joined_iterator(const iterator_type f0, const iterator_type l0, const iterator_type f1, const iterator_type l1) : _f0(f0), _l0(l0), _f1(f1), _l1(l1) {}

    struct iterator {
        using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
        using value_type = typename std::iterator_traits<iterator_type>::value_type;
        using pointer = typename std::iterator_traits<iterator_type>::pointer;
        using reference = typename std::iterator_traits<iterator_type>::reference;
        using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;

      protected:
        const joined_iterator *const _super;
        iterator_type _itr;
        internal::size_t _block = 0;

      public:
        explicit iterator(const joined_iterator *const super, const iterator_type itr, const internal::size_t block) : _super(super), _itr(itr), _block(block) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return *this->_itr; }

        inline iterator& operator++() noexcept(NO_EXCEPT) {
            if(++this->_itr == _super->_l0) this->_itr = _super->_f1, ++this->_block;
            return *this;
        }
        inline iterator& operator--() noexcept(NO_EXCEPT) {
            if(this->_itr-- == _super->_f1) this->_itr = std::prev(_super->l0), --this->_block;
            return *this;
        }

        inline iterator operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; return ++(*this), res; }
        inline iterator operator--(int) noexcept(NO_EXCEPT) { const auto res = *this; return --(*this), res; }

        friend inline bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return lhs._itr == rhs._itr; }
        friend inline bool operator!=(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return lhs._itr != rhs._itr; }

        friend inline difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) {
            if(lhs._block == rhs._block) return lhs._itr - rhs._itr;
            if(lhs._block > rhs._block) return (lhs._itr - lhs._super->_f1) + (rhs._super->_l0 - rhs._itr);
            if(lhs._block < rhs._block) return -(rhs - lhs);
            assert(false);
        }
    };

    using const_iterator = iterator;

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, this->_f0, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->_l1, 1); }

};


} // namespace lib
