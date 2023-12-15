#pragma once


#include <concepts>
#include <type_traits>
#include <atcoder/modint>


#include "snippet/aliases.hpp"

#include "internal/concepts.hpp"

#include "numeric/modular/builtin_reduction.hpp"
#include "numeric/modular/binary_reduction.hpp"
#include "numeric/modular/barrett_reduction.hpp"
#include "numeric/modular/montgomery_reduction.hpp"


namespace lib {

namespace internal {


template<class T>
concept modint_family =
    numeric<T> &&
    has_static_one<T> && has_static_zero<T> &&
    requires (T v, i64 p, typename T::value_type x) {
        { v.pow(p) } -> std::same_as<T>;
        { v.inv() } -> std::same_as<T>;
        { T::raw(x) } -> std::same_as<T>;

        { v.val() } -> std::same_as<typename T::value_type>;
        { T::mod() } -> std::same_as<typename T::value_type>;
        { T::max() } -> std::same_as<typename T::value_type>;
        T::digits;

        T::context::dynamic;
    };


template<class T>
concept dynamic_modint_family =
    modint_family<T> &&
    T::context::dynamic &&
    requires (typename T::value_type v) {
        T::set_mod(v);
    };


template<class T>
concept static_modint_family =
    modint_family<T> &&
    (!T::context::dynamic); //&&
    // requires {
    //     T::is_prime;
    // };


template<class T>
concept modular_reduction =
    std::default_initializable<T> &&
    std::constructible_from<T, typename T::value_type> &&
    requires (T v, typename T::value_type x, i64 p) {
        typename T::value_type;

        T::digits;
        { T::max() } -> std::same_as<typename T::value_type>;
        { v.mod() } -> std::same_as<typename T::value_type>;

        { v.reduce(x) } -> std::same_as<typename T::value_type>;

        { v.add(x, x) } -> std::same_as<typename T::value_type>;
        { v.subtract(x, x) } -> std::same_as<typename T::value_type>;
        { v.multiply(x, x) } -> std::same_as<typename T::value_type>;
        { v.multiply(x, x) } -> std::same_as<typename T::value_type>;
        { v.pow(x, p) } -> std::same_as<typename T::value_type>;

        { v.convert_raw(x) } -> std::same_as<typename T::value_type>;
        { v.convert(x) } -> std::same_as<typename T::value_type>;
        { v.revert(x) } -> std::same_as<typename T::value_type>;

        { v.equal(x, x) } -> std::same_as<bool>;
    };


template<class T>
concept modular_context =
    requires {
        typename T::reduction;
        typename T::value_type;
        { T::get() } -> std::same_as<std::add_lvalue_reference_t<std::add_const_t<typename T::reduction>>>;
    };


} // namespace internal


template<internal::modular_reduction Reduction, typename Reduction::value_type Mod>
struct static_modular_context {
    using reduction = Reduction;
    using value_type = typename reduction::value_type;

    static constexpr bool dynamic = false;

  private:
    using context = static_modular_context;

    static inline constexpr reduction _reduction = reduction(Mod);

  public:
    static constexpr const reduction& get() noexcept(NO_EXCEPT) { return context::_reduction; }
};


template<internal::modular_reduction Reduction, i64 Id>
struct dynamic_modular_context {
    using reduction = Reduction;
    using value_type = typename reduction::value_type;

    static constexpr bool dynamic = true;

  private:
    using context = dynamic_modular_context;

    static inline reduction _reduction;

  public:
    static constexpr void set_mod(const value_type mod) noexcept(NO_EXCEPT) { context::_reduction = reduction(mod); }

    static constexpr const reduction& get() noexcept(NO_EXCEPT) { return context::_reduction; }
};


template<internal::modular_context> struct modint;


template<u32 Mod> using static_builtin_modular_context_32bit = static_modular_context<builtin_reduction_32bit, Mod>;
template<u64 Mod> using static_builtin_modular_context_64bit = static_modular_context<builtin_reduction_64bit, Mod>;

template<u32 Mod> using static_barrett_modular_context_32bit = static_modular_context<barrett_reduction_32bit, Mod>;
template<u64 Mod> using static_barrett_modular_context_64bit = static_modular_context<barrett_reduction_64bit, Mod>;

template<u32 Mod> using static_montgomery_modular_context_32bit = static_modular_context<montgomery_reduction_32bit, Mod>;
template<u64 Mod> using static_montgomery_modular_context_64bit = static_modular_context<montgomery_reduction_64bit, Mod>;

template<u32 Mod> using static_arbitrary_montgomery_modular_context_32bit = static_modular_context<arbitrary_montgomery_reduction_32bit, Mod>;
template<u64 Mod> using static_arbitrary_montgomery_modular_context_64bit = static_modular_context<arbitrary_montgomery_reduction_64bit, Mod>;

template<u32 Mod> using static_binary_modular_context_32bit = static_modular_context<binary_reduction_32bit, Mod>;
template<u64 Mod> using static_binary_modular_context_64bit = static_modular_context<binary_reduction_64bit, Mod>;
template<u128 Mod> using static_binary_modular_context_128bit = static_modular_context<binary_reduction_128bit, Mod>;


template<u32 Mod> using static_builtin_modint_32bit = modint<static_builtin_modular_context_32bit<Mod>>;
template<u64 Mod> using static_builtin_modint_64bit = modint<static_builtin_modular_context_64bit<Mod>>;

template<u32 Mod> using static_barrett_modint_32bit = modint<static_barrett_modular_context_32bit<Mod>>;
template<u64 Mod> using static_barrett_modint_64bit = modint<static_barrett_modular_context_64bit<Mod>>;

template<u32 Mod> using static_montgomery_modint_32bit = modint<static_montgomery_modular_context_32bit<Mod>>;
template<u64 Mod> using static_montgomery_modint_64bit = modint<static_montgomery_modular_context_64bit<Mod>>;

template<u32 Mod> using static_arbitrary_montgomery_modint_32bit = modint<static_arbitrary_montgomery_modular_context_32bit<Mod>>;
template<u64 Mod> using static_arbitrary_montgomery_modint_64bit = modint<static_arbitrary_montgomery_modular_context_64bit<Mod>>;

template<u32 Mod> using static_binary_modint_32bit = modint<static_binary_modular_context_32bit<Mod>>;
template<u64 Mod> using static_binary_modint_64bit = modint<static_binary_modular_context_64bit<Mod>>;
template<u128 Mod> using static_binary_modint_128bit = modint<static_binary_modular_context_128bit<Mod>>;


template<i64 Id> using dynamic_builtin_modular_context_32bit = dynamic_modular_context<builtin_reduction_32bit, Id>;
template<i64 Id> using dynamic_builtin_modular_context_64bit = dynamic_modular_context<builtin_reduction_64bit, Id>;

template<i64 Id> using dynamic_barrett_modular_context_32bit = dynamic_modular_context<barrett_reduction_32bit, Id>;
template<i64 Id> using dynamic_barrett_modular_context_64bit = dynamic_modular_context<barrett_reduction_64bit, Id>;

template<i64 Id> using dynamic_montgomery_modular_context_32bit = dynamic_modular_context<montgomery_reduction_32bit, Id>;
template<i64 Id> using dynamic_montgomery_modular_context_64bit = dynamic_modular_context<montgomery_reduction_64bit, Id>;

template<i64 Id> using dynamic_arbitrary_montgomery_modular_context_32bit = dynamic_modular_context<arbitrary_montgomery_reduction_32bit, Id>;
template<i64 Id> using dynamic_arbitrary_montgomery_modular_context_64bit = dynamic_modular_context<arbitrary_montgomery_reduction_64bit, Id>;

template<i64 Id> using dynamic_binary_modular_context_32bit = dynamic_modular_context<binary_reduction_32bit, Id>;
template<i64 Id> using dynamic_binary_modular_context_64bit = dynamic_modular_context<binary_reduction_64bit, Id>;
template<i64 Id> using dynamic_binary_modular_context_128bit = dynamic_modular_context<binary_reduction_128bit, Id>;


template<i64 Id> using dynamic_builtin_modint_32bit = modint<dynamic_builtin_modular_context_32bit<Id>>;
template<i64 Id> using dynamic_builtin_modint_64bit = modint<dynamic_builtin_modular_context_64bit<Id>>;

template<i64 Id> using dynamic_barrett_modint_32bit = modint<dynamic_barrett_modular_context_32bit<Id>>;
template<i64 Id> using dynamic_barrett_modint_64bit = modint<dynamic_barrett_modular_context_64bit<Id>>;

template<i64 Id> using dynamic_montgomery_modint_32bit = modint<dynamic_montgomery_modular_context_32bit<Id>>;
template<i64 Id> using dynamic_montgomery_modint_64bit = modint<dynamic_montgomery_modular_context_64bit<Id>>;

template<i64 Id> using dynamic_arbitrary_montgomery_modint_32bit = modint<dynamic_arbitrary_montgomery_modular_context_32bit<Id>>;
template<i64 Id> using dynamic_arbitrary_montgomery_modint_64bit = modint<dynamic_arbitrary_montgomery_modular_context_64bit<Id>>;

template<i64 Id> using dynamic_binary_modint_32bit = modint<dynamic_binary_modular_context_32bit<Id>>;
template<i64 Id> using dynamic_binary_modint_64bit = modint<dynamic_binary_modular_context_64bit<Id>>;
template<i64 Id> using dynamic_binary_modint_128bit = modint<dynamic_binary_modular_context_128bit<Id>>;


template<u32 Mod> using static_modint_32bit = static_builtin_modint_32bit<Mod>;
template<u32 Mod> using static_modint_64bit = static_builtin_modint_64bit<Mod>;


using modint998244353 = static_modint_32bit<998244353>;
using modint1000000007 = static_modint_32bit<1000000007>;

using modint_32 = dynamic_barrett_modint_32bit<-1>;
using modint_64 = dynamic_barrett_modint_64bit<-1>;


template<const unsigned Val, const unsigned Mod = 998244353>
const lib::static_modint_32bit<Mod> MINT = Val;

template<const unsigned Val, const unsigned Mod = 998244353>
const unsigned INV = lib::static_modint_32bit<Mod>{ Val }.inv().val();

template<const unsigned Val, const unsigned Mod = 998244353>
const int SINV = lib::static_modint_32bit<Mod>{ Val }.inv().val();


} // namespace lib
