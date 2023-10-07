#pragma once


#include "internal/dev_env.hpp"


#include <optional>
#include <memory>
#include <type_traits>


#if CPP20


#include <ranges>
#include <concepts>


namespace lib {

namespace internal {


#if CPP23

template<class T>
concept boxable = std::copy_constructible<T> && std::is_object_v<T>;

#else

template<class T>
concept boxable = std::copy_constructible<T> && std::is_object_v<T>;

#endif


template<boxable T> struct box : std::optional<T> {
    using std::optional<T>::optional;

    constexpr box()
        noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::default_initializable<T>
        : std::optional<T> { std::in_place }
    {}

    box(const box&) = default;
    box(box&&) = default;

    using std::optional<T>::operator=;

    constexpr box &operator=(const box& other) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        requires(!std::copyable<T>) && std::copy_constructible<T>
    {
        if(this != std::addressof(other)) {
            if((bool)other) this->emplace(*other);
            else this->reset();
        }
        return *this;
    }

    constexpr box&
    operator=(box&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires(!std::movable<T>)
    {
        if(this != std::addressof(other)) {
            if((bool)other)
                this->emplace(std::move(*other));
            else
                this->reset();
        }
        return *this;
    }
};

template<class T>
concept boxable_copyable =
    std::copy_constructible<T> &&
    (
        std::copyable<T> ||
        (std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
    );

template<class T>
concept boxable_movable =
    (!std::copy_constructible<T>) &&
    (std::movable<T> || std::is_nothrow_move_constructible_v<T>);


template<boxable T>
    requires boxable_copyable<T> || boxable_movable<T>
struct box<T> {
  private:
    [[no_unique_address]] T _value = T();

  public:
    box() requires std::default_initializable<T> = default;

    constexpr explicit box(const T &__t) noexcept(
        std::is_nothrow_copy_constructible_v<T>
    )
        requires std::copy_constructible<T>
    : _value(__t)
    {}

    constexpr explicit box(T &&__t) noexcept(
        std::is_nothrow_move_constructible_v<T>
    )
    : _value(std::move(__t)) {}

    template<class... Args>
        requires std::constructible_from<T, Args...>
    constexpr explicit box(std::in_place_t, Args &&...args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>
    )
    : _value(std::forward<Args>(args)...) {}

    box(const box &) = default;
    box(box &&) = default;
    box &operator=(const box &) requires std::copyable<T> = default;
    box &operator=(box &&) requires std::movable<T> = default;

    constexpr box &operator=(const box &other) noexcept
        requires(!std::copyable<T>) && std::copy_constructible<T>
    {
        static_assert(std::is_nothrow_copy_constructible_v<T>);
        if(this != std::addressof(other)) {
            _value.~T();
            std::construct_at(std::addressof(_value), *other);
        }
        return *this;
    }

    constexpr box &operator=(box &&other) noexcept
        requires(!std::movable<T>)
    {
        static_assert(std::is_nothrow_move_constructible_v<T>);
        if(this != std::addressof(other)) {
            _value.~T();
            std::construct_at(std::addressof(_value), std::move(*other));
        }
        return *this;
    }

    constexpr bool has_value() const noexcept { return true; };
    constexpr T &operator*() noexcept { return _value; }
    constexpr const T &operator*() const noexcept { return _value; }
    constexpr T *operator->() noexcept { return std::addressof(_value); }
    constexpr const T *operator->() const noexcept { return std::addressof(_value); }
};


}  // namespace internal

} // namespace lib

#endif
