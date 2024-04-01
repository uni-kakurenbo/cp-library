#pragma once


#include <iterator>

#include "snippet/aliases.hpp"
#include "debugger/debug.hpp"


namespace lib {

namespace internal {


template<class Derived, class Interface, class Operand, bool LEAF_ONLY>
struct enumerable_tree {
  private:
    using node_handler = typename Interface::node_handler;
    using node_pointer = typename Interface::node_pointer;

    using size_type = typename Interface::node_pointer;


    inline auto _push(const node_pointer& tree) {
        return static_cast<Derived*>(this)->push(tree);
    }

  public:
    template<std::forward_iterator I>
        requires std::output_iterator<I, Operand>
    void enumerate(node_pointer tree, I& itr) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) return;

        this->_push(tree);

        this->enumerate(tree->left, itr);

        if constexpr(LEAF_ONLY) {
            if(tree->is_leaf()) {
                REP(tree->size) *(itr++) = tree->data.val;
            }
        }
        else {
            REP(tree->length) *(itr++) = tree->data.val;
        }

        this->enumerate(tree->right, itr);
    }
};


} // namespace internal


} // namespace lib
