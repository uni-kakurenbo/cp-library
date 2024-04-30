#pragma once


#include <cstdint>
#include <chrono>
#include <algorithm>


#include "internal/dev_env.hpp"


namespace uni {


struct timer {
    using time_point = std::chrono::milliseconds::rep;
    using progress_type = long double;

  private:
    time_point _time_limit = 0;
    progress_type progress_duration = 0;

    std::chrono::system_clock::time_point clock_start, clock_end;

  public:
    timer() noexcept = default;

    explicit timer(const time_point time_limit) noexcept(NO_EXCEPT) { this->reset(time_limit); }

    inline time_point limit() noexcept(NO_EXCEPT) { return this->_time_limit; }

    inline timer& reset() noexcept(NO_EXCEPT) {
        this->clock_start = std::chrono::system_clock::now();
        this->clock_end = clock_start + std::chrono::milliseconds(this->_time_limit);
        return *this;
    }

    inline timer& reset(const time_point time_limit) noexcept(NO_EXCEPT) {
        this->_time_limit = time_limit;

        this->progress_duration =
            static_cast<progress_type>(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(
                    std::chrono::milliseconds(time_limit)
                ).count()
            );

        return this->reset();
    }

    inline time_point elapsed() const noexcept(NO_EXCEPT) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->clock_start).count();
    }
    inline time_point remaining() const noexcept(NO_EXCEPT) {
        return _time_limit - this->elapsed();
    }

    inline bool expired() const noexcept(NO_EXCEPT) { return this->elapsed() >= _time_limit; }
    inline progress_type progress() const noexcept(NO_EXCEPT) {
        return std::clamp<progress_type>((std::chrono::system_clock::now() - this->clock_start).count() / this->progress_duration, 0, 1);
    }
};


} // namespace uni
