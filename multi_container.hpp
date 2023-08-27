#pragma once


#include <cassert>
#include <vector>
#include <array>

#if CPP20

#include <concepts>

#endif

#include "internal/dev_env.hpp"
#include "internal/exception.hpp"
#include "internal/types.hpp"

#include "adapter/valarray.hpp"


namespace lib {

namespace internal {

namespace multi_container_impl {


template<class container> struct base : container {
    using container::container;

  protected:
    inline void _validate_index(__attribute__ ((unused)) const internal::size_t index) const noexcept(NO_EXCEPT) {
        assert(0 <= index and index < (internal::size_t)this->size());
    }
    inline internal::size_t _positivize_index(const internal::size_t x) const noexcept(NO_EXCEPT) {
        return x < 0 ? this->size() + x : x;
    }
};


} // namespace multi_contatiner_impl

} // namespace internal


template<class T, const unsigned int RANK, template<class...> class container = valarray>
struct multi_container : internal::multi_container_impl::base<container<multi_container<T,RANK-1,container>>> {
    using internal::multi_container_impl::base<container<multi_container<T,RANK-1,container>>>::base;

#if CPP20
    template<std::integral Head, class... Tail>
    multi_container(const Head head, const Tail... tail) noexcept(NO_EXCEPT)
    : internal::multi_container_impl::base<container<multi_container<T,RANK-1,container>>>(head, multi_container<T,RANK-1,container>(tail...)) { assert(head >= 0); }

#else
    template<class Head, class... Tail>
    multi_container(const Head head, const Tail... tail) noexcept(NO_EXCEPT)
    : internal::multi_container_impl::base<container<multi_container<T,RANK-1,container>>>(head, multi_container<T,RANK-1,container>(tail...)) {
        static_assert(std::is_integral_v<Head>, "size type must be integral");
    }
#endif

    template<class Head, class... Tail> T& operator()(const Head _head, const Tail... tail) noexcept(NO_EXCEPT) {
        static_assert(std::is_integral_v<Head>, "index must be integral");

        const internal::size_t head = this->_positivize_index(_head);
        this->_validate_index(head);
        return (*this)[head](tail...);
    }

    template<class Head, class... Tail> const T& operator()(const Head _head, const Tail... tail) const noexcept(NO_EXCEPT) {
        static_assert(std::is_integral_v<Head>, "index must be integral");

        const internal::size_t head = this->_positivize_index(_head);
        this->_validate_index(head);
        return (*this)[head](tail...);
    }
};

template<class T, template<class...> class container>
struct multi_container<T,1,container> : internal::multi_container_impl::base<container<T>> {
    using internal::multi_container_impl::base<container<T>>::base;

    template<class... Args> multi_container(const Args&... args) noexcept(NO_EXCEPT) : internal::multi_container_impl::base<container<T>>(args...) {}

    T& operator()(const internal::size_t _index) noexcept(NO_EXCEPT) {
        const internal::size_t index = this->_positivize_index(_index);
        this->_validate_index(index);
        return (*this)[index];
    }
    const T& operator()(const internal::size_t _index) const noexcept(NO_EXCEPT) {
        const internal::size_t index = this->_positivize_index(_index);
        this->_validate_index(index);
        return (*this)[index];
    }
};


// template<class T, const unsigned int RANK, class base = std::vector<T>>
// struct UnfoldedMultiContainer : base {
//   protected:
//     std::array<size_t,RANK> size_list;

//   public:
//     using base::base;

//     template<class... Args>
//     UnfoldedMultiContainer(const Args... _args) {
//         const std::initializer_list<size_t> args { _args... };

//         dev_debug(args.size() == RANK or args.size() == RANK + 1);

//         size_t length = 0;
//         REP(r, RANK) length += size_list[r] = args[r];

//         if(args.size() == RANK+1) this->assign(length, args.back());
//         else this->assign(length, T{});
//     }

//     template<class... Args> T& operator()(Args... args) {
//         const std::initializer_list<size_t> args { _args... };
//         reverse(ALL(args));

//         assert(args.size() == RANK);

//         size_t curr = 0;
//         ITR(r, RANK) {
//             curr += arg[r];
//             curr += arg[r] * size_list[];
//         }
//     }

//     template<class Head, class... Tail> const T& operator()(Head head, Tail... tail) const noexcept(NO_EXCEPT) {
//     }
// };


template<class T, template<class...> class container>
struct multi_container<T,0,container> {
    static_assert(internal::EXCEPTION<T>, "invalid rank: 0, should be 1 or more");
};

// template<class T, class container>
// struct UnfoldedMultiContainer<T,0,container> {
//     static_assert(internal::EXCEPTION<T>, "invalid rank: 0, should be 1 or more");
// };

} // namespace lib
