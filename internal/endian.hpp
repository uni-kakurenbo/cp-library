#pragma once


#include <cstdint>
#include <optional>


namespace lib {

namespace internal {


enum class endian : std::uint8_t {
    little,
    big,
    other
};


endian discern_endian() {
  #ifdef __BYTE_ORDER__

    if constexpr(__BYTE_ORDER == __BIG_ENDIAN) return endian::big;
    if constexpr(__BYTE_ORDER == __LITTLE_ENDIAN) return endian::little;
    return {};

  #else
    static std::optional<endian> res;
    if(res) return res.value();

    union U {
        short s;
        char c[2];
    };
    U u = { .s = 0x0001 };

    if(u.c[0] == 0x00 && u.c[1] == 0x01) return *(res = endian::big);
    if(u.c[0] == 0x01 && u.c[1] == 0x00) return *(res = endian::little);
    return *(res = endian::other);

  #endif
}


} // namespace internal

} // namespace lib
