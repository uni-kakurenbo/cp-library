
#ifndef INCLUDED_RESOLVING_RANK // for debuggers
#define INCLUDED_RESOLVING_RANK


namespace lib {

namespace internal {


template<int P> struct resolving_rank : resolving_rank<P-1> {};
template<> struct resolving_rank<0> {};


} // namespace internal

} // namespace lib


#endif //ifndef INCLUDED_RESOLVING_RANK
