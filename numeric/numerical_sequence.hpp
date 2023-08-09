#pragma once


#include "internal/types.hpp"
#include "internal/dev_env.hpp"

#include "numeric/arithmetic.hpp"


namespace lib {

namespace numerical_sequence {


namespace internal {


using size_t = std::int64_t;


template<class T>
struct single_constant {
    using value_type = T;

  private:
    value_type _val;

  public:
    constexpr single_constant() = default;
    constexpr single_constant(const value_type& val) noexcept(NO_EXCEPT) : _val(val) {}

    inline constexpr const value_type& val() const noexcept(NO_EXCEPT) { return this->_val; }
    inline constexpr operator value_type() const noexcept(NO_EXCEPT) { return this->_val; }
};


} // namespace internl


template<class T>
struct term {
    using size_type = internal::size_t;
    using value_type = T;

  private:
    size_type _index;
    value_type _val;

  public:
    constexpr term() = default;
    constexpr explicit term(const size_type index, const value_type& val) noexcept(NO_EXCEPT) : _index(index), _val(val) {}

    inline constexpr size_type index() const noexcept(NO_EXCEPT) { return this->_index; }
    inline constexpr const value_type& val() const noexcept(NO_EXCEPT) { return this->_val; }

};

template<class T, internal::size_t INDEX>
struct static_term : internal::single_constant<T> {
    using internal::single_constant<T>::single_constant;

    using size_type = internal::size_t;
    using value_type = T;

    inline constexpr size_type index() const noexcept(NO_EXCEPT) { return INDEX; }
    inline constexpr operator term<value_type>() const noexcept(NO_EXCEPT) { return term(INDEX, this->_val); }
};

template<class T>
struct common_difference : internal::single_constant<T> {
    using internal::single_constant<T>::single_constant;
};

template<class T>
struct common_ratio : internal::single_constant<T> {
    using internal::single_constant<T>::single_constant;
};


template<class T>
struct arithmetic {
    using size_type = internal::size_t;
    using value_type = T;

  protected:
    value_type _begin, _diff;

  public:
    constexpr arithmetic() = default;
    constexpr arithmetic(const static_term<value_type,0>& begin, const common_difference<value_type>& diff)
    : _begin(static_cast<value_type>(begin.val())), _diff(diff.val()) {}
    constexpr arithmetic(const common_difference<value_type>& diff, const static_term<value_type,0>& begin) : arithmetic(begin, diff) {}

    constexpr arithmetic(const term<value_type>& p, const common_difference<value_type>& diff) noexcept(NO_EXCEPT)
      : arithmetic(diff, static_cast<value_type>(p.val() - diff * p.index()))
    {}
    constexpr arithmetic(const common_difference<value_type>& diff, const term<value_type>& p) noexcept(NO_EXCEPT) : arithmetic(p, diff) {}

    constexpr arithmetic(const term<value_type> p, const term<value_type> q) noexcept(NO_EXCEPT)
      : arithmetic(p, common_difference(static_cast<value_type>((p.val() - q.val()) / (p.index() - q.index()))))
    {}

    inline constexpr term<value_type> operator[](const size_type k) const noexcept(NO_EXCEPT) {
        return term(k, static_cast<value_type>(this->_begin + this->_diff * k));
    }
    inline constexpr value_type sum(const size_type k) const noexcept(NO_EXCEPT) {
        return static_cast<value_type>((this->_begin * k) + (k - 1) * k / 2 * this->_diff);
    }
    inline constexpr value_type sum(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        return this->sum(r) - this->sum(l);
    }
};



template<class T>
struct geometric {
    using size_type = internal::size_t;
    using value_type = T;

  protected:
    value_type _begin, _ratio;

  public:
    constexpr geometric() = default;
    constexpr geometric(const static_term<value_type,0> begin, const common_ratio<T> ratio) noexcept(NO_EXCEPT) : _begin(begin), _ratio(ratio) {};

    inline constexpr term<value_type> operator[](const size_type k) const noexcept(NO_EXCEPT) {
        return term(k, this->_begin * lib::pow(this->_diff, k));
    }
    inline constexpr value_type sum(const size_type k) const noexcept(NO_EXCEPT) {
        if(this->_ratio == 1) return this->_begin * this->k;
        else {
            return this->_begin * (lib::pow(this->_ratio, k) - 1) / (this->_ratio - 1);
        }
        return ;
    }
    inline constexpr value_type sum(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        return this->sum(r) - this->sum(l);
    }
};


} // namespace numerical_sequence

} // namespace lib
