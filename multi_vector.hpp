#pragma once

#include <vector>
#include <array>

#include "internal/develop/dev_assert.hpp"
#include "internal/exception.hpp"
#include "internal/types.hpp"

namespace Internal {
    namespace MultiVectorLib {
        template<class Container> struct Base : Container {
            using Container::Container;

          protected:
            inline void _validate_index(__attribute__ ((unused)) const Internal::Size index) const {
                dev_assert(0 <= index and index < (Internal::Size)this->size());
            }
            inline Internal::Size _positivize_index(const Internal::Size x) const {
                return x < 0 ? this->size() + x : x;
            }
        };
    }
}

template<class T, const unsigned int RANK, template<class...> class Container = std::vector>
struct MultiVector : Internal::MultiVectorLib::Base<Container<MultiVector<T,RANK-1,Container>>> {
    using Internal::MultiVectorLib::Base<Container<MultiVector<T,RANK-1,Container>>>::Base;

    template<class Head, class... Tail>
    MultiVector(const Head head, const Tail&&... tail)
    : Internal::MultiVectorLib::Base<Container<MultiVector<T,RANK-1,Container>>>(head, MultiVector<T,RANK-1,Container>(std::forward<const Tail>(tail)...)) {
        static_assert(is_integral_v<Head>, "size must be integral");
    }

    template<class Head, class... Tail> T& operator()(const Head _head, const Tail... tail) {
        static_assert(is_integral_v<Head>, "index must be integral");

        const Internal::Size head = this->_positivize_index(_head);
        this->_validate_index(head);
        return (*this)[head](tail...);
    }

    template<class Head, class... Tail> const T& operator()(const Head _head, const Tail... tail) const {
        static_assert(is_integral_v<Head>, "index must be integral");

        const Internal::Size head = this->_positivize_index(_head);
        this->_validate_index(head);
        return (*this)[head](tail...);
    }
};

template<class T, template<class...> class Container>
struct MultiVector<T,1,Container> : Internal::MultiVectorLib::Base<Container<T>> {
    using Internal::MultiVectorLib::Base<Container<T>>::Base;

    template<class... Args> MultiVector(const Args&&... args) : Internal::MultiVectorLib::Base<Container<T>>(std::forward<const Args>(args)...) {}

    T& operator()(const Internal::Size _index) {
        const Internal::Size index = this->_positivize_index(_index);
        this->_validate_index(index);
        return (*this)[index];
    }
    const T& operator()(const Internal::Size _index) const {
        const Internal::Size index = this->_positivize_index(_index);
        this->_validate_index(index);
        return (*this)[index];
    }
};


// template<class T, const unsigned int RANK, class Base = std::vector<T>>
// struct UnfoldedMultiVector : Base {
//   protected:
//     std::array<Size,RANK> size_list;

//   public:
//     using Base::Base;

//     template<class... Args>
//     UnfoldedMultiVector(const Args... _args) {
//         const std::initializer_list<Size> args { _args... };

//         dev_debug(args.size() == RANK or args.size() == RANK + 1);

//         Size length = 0;
//         REP(r, RANK) length += size_list[r] = args[r];

//         if(args.size() == RANK+1) this->assign(length, args.back());
//         else this->assign(length, T{});
//     }

//     template<class... Args> T& operator()(Args... args) {
//         const std::initializer_list<Size> args { _args... };
//         reverse(ALL(args));

//         dev_assert(args.size() == RANK);

//         Size curr = 0;
//         ITR(r, RANK) {
//             curr += arg[r];
//             curr += arg[r] * size_list[];
//         }
//     }

//     template<class Head, class... Tail> const T& operator()(Head head, Tail... tail) const {
//     }
// };


template<class T, template<class...> class Container>
struct MultiVector<T,0,Container> {
    static_assert(TYPE_EXCEPTION<T>, "invalid rank: 0, should be 1 or more");
};

// template<class T, class Container>
// struct UnfoldedMultiVector<T,0,Container> {
//     static_assert(TYPE_EXCEPTION<T>, "invalid rank: 0, should be 1 or more");
// };
