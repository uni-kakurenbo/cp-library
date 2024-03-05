#pragma once


#include <cassert>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <random>
#include <concepts>
#include <ranges>


#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"
#include "internal/uncopyable.hpp"
#include "internal/dummy.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "random/xorshift.hpp"


namespace lib {

namespace internal {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class Derived, std::integral KeyType, std::integral SizeType, bool EXPLICIT>
struct treap_interface : private uncopyable {
    using key_type = KeyType;
    using size_type = SizeType;

  public:
    template<class>
    struct node_type;

  private:
    static constexpr i64 XORSHIFT_ID = -(1L << 62);
    static inline xorshift<XORSHIFT_ID> rand;

    using priority_type = xorshift<>::result_type;


    inline Derived* _derived() noexcept(NO_EXCEPT) {
        return static_cast<Derived*>(this);
    }

    inline const Derived* _derived() const noexcept(NO_EXCEPT) {
        return static_cast<const Derived*>(this);
    }


    template<pointer Tree>
    inline void update_size(const Tree tree) const noexcept(NO_EXCEPT) { if(tree) tree->size = 1 + this->size(tree->left) + this->size(tree->right); }

    template<pointer Tree>
    inline void pushup(const Tree tree) const noexcept(NO_EXCEPT) {
        this->update_size(tree);
        this->_derived()->pushup(tree);
    }

  public:
    template<pointer Tree>
    inline size_type size(const Tree tree) const noexcept(NO_EXCEPT) { return tree ? tree->size : 0; }

    template<class DataType = dummy>
    struct node_type : DataType {
        using data_type = DataType;

        priority_type priority;
        [[no_unique_address]] std::conditional_t<EXPLICIT, KeyType, dummy> key;

        std::add_pointer_t<node_type> left = nullptr, right = nullptr;

        size_type size = 1;


        node_type(const data_type& data, const key_type _key, const treap_interface* super) noexcept(NO_EXCEPT)
            requires (EXPLICIT)
          : data_type(data), priority(super->rand()), key(_key)
        {}

        node_type(const data_type& data, const key_type, const treap_interface* super) noexcept(NO_EXCEPT)
            requires (!EXPLICIT)
          : data_type(data), priority(super->rand())
        {}

        ~node_type() {
            if(this->left) delete this->left;
            if(this->right) delete this->right;
        }
    };


    template<pointer Tree, pointer Left, pointer Right>
    void split(const Tree tree, const key_type pos, Left& left, Right& right) const noexcept(NO_EXCEPT) {
        if(!tree) {
            left = right = nullptr;
            return;
        }

        this->_derived()->pushdown(tree);

        if constexpr(EXPLICIT) {
            if(pos <= tree->key) {
                this->split(tree->left, pos, left, tree->left), right = std::move(tree);
                this->pushup(right);
            }
            else {
                this->split(tree->right, pos, tree->right, right), left = std::move(tree);
                this->pushup(left);
            }
        }
        else {
            key_type key = this->size(tree->left) + 1;

            if(pos < key) {
                this->split(tree->left, pos, left, tree->left), right = std::move(tree);
                this->pushup(right);
            }
            else {
                this->split(tree->right, pos - key, tree->right, right), left = std::move(tree);
                this->pushup(left);
            }
        }
    }


    template<pointer Tree, pointer Left, pointer Right>
    void merge(Tree& tree, const Left left, const Right right) const noexcept(NO_EXCEPT) {
        this->_derived()->pushdown(left);
        this->_derived()->pushdown(right);

        if(!(left && right)) {
            tree = left ? left : right;
        }
        else if(left->priority > right->priority) {
            this->merge(left->right, left->right, right), tree = std::move(left);
        }
        else {
            this->merge(right->left, left, right->left), tree = std::move(right);
        }

        this->pushup(tree);
    }
};


} // namespace internal


template<bool EXPLICIT = false, std::integral KeyType = internal::size_t>
struct treap_context {
    inline static constexpr bool IMPLICIT_KEY = !EXPLICIT;

    template<class Derived, std::integral SizeType>
    using interface = internal::treap_interface<Derived, KeyType, SizeType, EXPLICIT>;
};


} // namespace lib
