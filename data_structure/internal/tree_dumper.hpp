#pragma once


#include <string>


#include "snippet/aliases.hpp"
#include "debugger/debug.hpp"


namespace lib {

namespace internal {


template<class Derived, class Core, bool LEAF_ONLY>
struct dumpable_tree {
  private:
    using node_handler = typename Core::node_handler;
    using node_pointer = typename Core::node_pointer;

    using size_type = typename Core::node_pointer;

    inline auto _push(const node_pointer& tree) {
        return static_cast<Derived*>(this)->_impl.push(tree);
    }

  public:
    debugger::debug_t dump_rich(node_pointer tree, const std::string prefix, const int dir, size_type& index)
        requires (!LEAF_ONLY)
    {
        if(!tree || tree == node_handler::nil) return prefix + "\n";

        this->_push(tree);

        const auto left = this->dump_rich(tree->left, prefix + (dir == 1 ? "| " : "  "), -1, index);
        const auto here = prefix + "--+ " + debugger::dump(index) + " : " + debugger::dump(tree->data) + " [" + debugger::dump(tree->length) + "]\n";
        index += tree->length;

        const auto right = this->dump_rich(tree->right, prefix + (dir == -1 ? "| " : "  "), 1, index);

        return left + here + right;
    }

    debugger::debug_t dump_rich(node_pointer tree, const std::string prefix, const int dir, size_type& index)
        requires
            (
                LEAF_ONLY &&
                requires {
                    typename Core::node_colors;
                }
            )
    {
        if(!tree || tree == node_handler::nil) return prefix + "\n";

        this->_push(tree);

        const auto left = this->dump_rich(tree->left, prefix + (dir == 1 ? "| " : "  "), -1, index);
        const auto right = this->dump_rich(tree->right, prefix + (dir == -1 ? "| " : "  "), 1, index);


        const auto color = tree->color == Core::node_colors::BLACK ? "<->" : "<+>";

        const auto here = [&]() -> std::string {
            if(tree->is_leaf()) {
                index += tree->size;

                return
                    prefix + "--+ [" +
                    debugger::dump(index - tree->size) + ", " + debugger::dump(index) + ") : " +
                    debugger::COLOR_STRING + color + debugger::COLOR_INIT + " " +
                    debugger::dump(tree->data) + "\n";
            }
            return "";
        }();

        return left + here + right;
    }


    inline debugger::debug_t dump_rich(const node_pointer& tree, const std::string prefix = "   ", const int dir = 0) {
        size_type index = 0;
        return this->dump_rich(tree, prefix, dir, index);
    }


    debugger::debug_t _debug(node_pointer tree)
        requires (!LEAF_ONLY)
    {
        if(!tree || tree == node_handler::nil) return "";

        this->_push(tree);

        return
            "(" +
            this->_debug(tree->left) + " " +
            debugger::dump(tree->data) + " [" +
            debugger::dump(tree->length) + "] " +
            this->_debug(tree->right) +
            ")";
    }

    debugger::debug_t _debug(node_pointer tree)
        requires LEAF_ONLY
    {
        if(!tree || tree == node_handler::nil) return "";

        this->_push(tree);

        return
            "(" +
            this->_debug(tree->left) + " " +
            (
                tree->is_leaf()
                    ?
                        debugger::dump(tree->data) + " [" +
                        debugger::dump(tree->size) + "] "
                    :
                        ""
            ) +
            this->_debug(tree->right) +
            ")";
    }
};


} // namespace internal


} // namespace lib
