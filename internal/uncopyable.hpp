#pragma once


namespace uni {

namespace internal {


struct uncopyable {
    uncopyable() noexcept {}
    uncopyable(const uncopyable&) = delete;
    uncopyable& operator=(const uncopyable&) = delete;
};


} // namespace internal

} // namespace uni
