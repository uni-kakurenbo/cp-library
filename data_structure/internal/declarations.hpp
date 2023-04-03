#pragma once


namespace lib {

namespace internal {

namespace fenwick_tree_lib {

template<class> struct core;

} // namespace implici_treap_lib


namespace segment_tree_lib {

template<class,class> struct core;

} // namespace segment_tree_lib


namespace lazy_segment_tree_lib {

template<class> struct core;

} // namespace lazy_segment_tree_lib


namespace implicit_treap_lib {

template<class> struct core;

} // namespace implici_treap_lib


namespace disjoint_sparse_table_lib {

template<class,class> struct core;

} // namespace disjoint_sparse_table_lib


} // namespace internal


template<class> struct fenwick_tree;
template<class> struct segment_tree;
template<class> struct lazy_segment_tree;
template<class> struct implicit_treap;
template<class> struct disjoint_sparse_table;


} // namespace lib
