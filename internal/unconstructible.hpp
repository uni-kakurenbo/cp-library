#pragma once


namespace uni {

namespace internal {


struct unconstructible  {
  private:
    template<class... Args>
    unconstructible(Args...) = delete;
};


} // namespace internal

} // namespace uni
