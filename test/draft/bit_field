#include <cstdint>
#include <utility>
#include <type_traits>

namespace internal {

namespace tags {

template<class Flag> struct base {
    using flag = Flag;

    using bit_type = typename std::underlying_type_t<flag>;

    const bit_type _bits;
    constexpr bit_type bits() const { return this->_bits; }

    template<class... Tail>
    static constexpr bit_type set(flag head, Tail... tail) {
        return static_cast<bit_type>(head) | set(tail...);
    }

    static constexpr bit_type set() { return 0; }


    template<class... Flags>
    constexpr base(Flags... flags) : _bits(set(flags...)) {}


    template<class... Tail> constexpr bool has(flag head, Tail... tail) {
        return (this->bits() & static_cast<bit_type>(head)) and has(tail...);
    }
    constexpr bool has() { return true; }
};

}

}



enum struct flag : std::int32_t {
    fenwick_tree = 1 << 0,
    segment_tree = 1 << 1,
    lazy_segment_tree = 1 << 2,
    implicit_treap = 1 << 3,
};

struct tags : internal::tags::base<flag> {
    using internal::tags::base<flag>::base;
};



#include <iostream>


signed main() {
    tags data(tags::flag::fenwick_tree, tags::flag::implicit_treap);
    std::cout << data.bits() << "\n";
    std::cout << data.has(tags::flag::fenwick_tree) << "\n";
    std::cout << data.has(tags::flag::implicit_treap) << "\n";
    std::cout << data.has(tags::flag::segment_tree) << "\n";
    std::cout << data.has(tags::flag::lazy_segment_tree) << "\n";
}
