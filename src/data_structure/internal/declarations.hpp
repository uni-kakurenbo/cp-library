#pragma once


#include "data_structure/range_action/base.hpp"


namespace lib {

namespace internal {

namespace fenwick_tree_impl {

template<class> struct core;

} // namespace implici_treap_impl


namespace segment_tree_impl {

template<class> struct core;

} // namespace segment_tree_impl


namespace lazy_segment_tree_impl {

template<class> struct core;

} // namespace lazy_segment_tree_impl


namespace implicit_treap_impl {

template<actions::internal::full_action> struct core;

} // namespace implici_treap_impl


namespace disjoint_sparse_table_impl {

template<class> struct core;

} // namespace disjoint_sparse_table_impl


} // namespace internal


template<class> struct fenwick_tree;
template<class> struct segment_tree;
template<class> struct lazy_segment_tree;
template<class> struct implicit_treap;
template<class> struct disjoint_sparse_table;


} // namespace lib
