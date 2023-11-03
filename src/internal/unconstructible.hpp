#pragma once


namespace lib {

namespace internal {


struct unconstructible  {
  private:
    template<class... Args>
    unconstructible(Args...) = delete;
};


} // namespace internal

} // namespace lib
