#pragma once


#include <cassert>
#include <iterator>
#include <vector>
#include <utility>
#include <functional>
#include <valarray>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/exception.hpp"


namespace uni {

namespace internal {

namespace interval_scanner_impl {


template<class T> using interval = std::pair<T, T>;
template<class T> using intervals = std::vector<std::pair<T, T>>;


template<class T>
struct base {
  protected:
    std::function<bool(T)> _validate;

  public:
    base(std::function<bool(T)> validate) : _validate(validate) {}

    void scan(T, T, T) {
        static_assert(internal::EXCEPTION_ON_TYPE<T>, "not implemented: scan()");
    }

    void split(const T first, const T last, intervals<T> *intervals) const {
        std::valarray<bool> _valid(false, last - first);
        for(auto itr=first,index=0; itr!=last; ++itr, ++index) _valid[index] = _validate(itr);

        auto can_begin = [&](const T itr) {
            const auto index = itr - first;
            if(itr == first) return _valid[index];
            if(itr == last) return false;
            if(not _valid[index-1] and _valid[index]) return true;
            return false;
        };

        auto is_end = [&](const T itr) {
            const  auto index = itr - first;
            if(itr == first) return false;
            if(itr == last) return _valid[index-1];
            if(_valid[index-1] and not _valid[index]) return true;
            return false;
        };

        {
            intervals->clear();
            T start = first;
            for(auto itr=first; ; ++itr) {
                if(can_begin(itr)) start = itr;
                if(is_end(itr)) intervals->emplace_back(start, itr);
                if(itr == last) break;
            }
        }
    }

    void scan_all(const T first, const T last) const {
        intervals<T> targets;
        this->split(first, last, &targets);
        ITR(start, end, targets) this->scan(first, start, end);
    }
};


} // namespace interval_scanner_impl

} // namespace internal


template<class T>
struct exclusive_interval_scanner : internal::interval_scanner_impl::base<T> {
  private:
    std::function<void(T)> _init;
    std::function<bool(T)> _can_expand;
    std::function<void(T)> _expand, _contract;
    std::function<void(T, T)> _apply;

  public:
    using interval = internal::interval_scanner_impl::interval<T>;
    using intervals = internal::interval_scanner_impl::intervals<T>;

    exclusive_interval_scanner(
        std::function<bool(T)> validate,
        std::function<void(T)> init,
        std::function<bool(T)> can_expand,
        std::function<void(T)> expand,
        std::function<void(T)> contract,
        std::function<void(T, T)> apply
    )
      : internal::interval_scanner_impl::base<T>(validate), _init(init), _can_expand(can_expand),
        _expand(expand), _contract(contract), _apply(apply)
    {}

    template<const bool FOLLOWING = true>
    void scan(const T start, const T end) const {
        T l_itr=start, r_itr=start;
        while(l_itr < end) {
            if (FOLLOWING and r_itr <= l_itr) {
                r_itr = l_itr+1;
                _init(l_itr);
            }
            while(r_itr < end && _can_expand(r_itr)) {
                _expand(r_itr++);
            }
            _apply(l_itr, r_itr);
            _contract(l_itr++);
        }
    };

    template<const bool FOLLOWING = true>
    void scan_all(const T first, const T last) const {
        intervals targets;
        this->split(first, last, &targets);
        ITR(start, end, targets) this->scan<FOLLOWING>(start, end);
    }
};


template<class T>
struct inclusive_interval_scanner : internal::interval_scanner_impl::base<T> {
  protected:
    std::function<void()> _init;
    std::function<bool()> _valid;
    std::function<void(T)> _expand, _contract;
    std::function<void(T, T)> _apply;

  public:
    using interval = internal::interval_scanner_impl::interval<T>;
    using intervals = internal::interval_scanner_impl::intervals<T>;

    inclusive_interval_scanner(
        std::function<bool(T)> validate,
        std::function<void()> init,
        std::function<bool()> valid,
        std::function<void(T)> expand,
        std::function<void(T)> contract,
        std::function<void(T, T)> apply
    ) : internal::interval_scanner_impl::base<T>(validate), _init(init), _valid(valid), _expand(expand), _contract(contract), _apply(apply) {}

    template<const bool INVERSE = false, const bool FOLLOWING = true, const bool CONFIRMATION = true>
    void scan(const T start, const T end) const {
        T l_itr = start, r_itr = start;
        _init();
        while(l_itr < end) {
            if(FOLLOWING and r_itr < l_itr) {
                r_itr = l_itr;
                _init();
            }
            if(r_itr < end and (INVERSE ^ _valid())) {
                _expand(r_itr++);
            }
            else {
                _contract(l_itr++);
            }
            if(!CONFIRMATION or _valid()) _apply(l_itr, r_itr);
        }
    }

    template<const bool INVERSE = false, const bool FOLLOWING = true, const bool CONFIRMATION = true>
    void scan_all(const T first, const T last) const {
        intervals targets;
        this->split(first, last, &targets);
        ITR(start, end, targets) this->scan<INVERSE,FOLLOWING,CONFIRMATION>(start, end);
    }
};


} // namespace uni
