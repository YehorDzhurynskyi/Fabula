#pragma once

#include <cstdint>
#include <cmath>
#include <type_traits>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

template<typename T>
struct vec2
{
    static_assert(std::is_arithmetic<T>::value, "should be an arithmetic type");

    T x;
    T y;

    constexpr vec2() : vec2(T(0), T(0)) {}
    constexpr vec2(T x, T y) : x(x), y(y) {}

    constexpr vec2(const vec2<T>& rhs) = default;
    vec2& operator=(const vec2<T>& rhs) = default;

#pragma region Unary

    constexpr vec2<T> operator-() const
    {
        return { -x, -y };
    }

    template<typename U>
    vec2<T>& operator+=(U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");

        x += rhs;
        y += rhs;

        return *this;
    }

    vec2<T>& operator+=(const vec2<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    template<typename U>
    vec2<T>& operator-=(U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");

        x -= rhs;
        y -= rhs;

        return *this;
    }

    vec2<T>& operator-=(const vec2<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    template<typename U>
    vec2<T>& operator*=(U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");

        x *= rhs;
        y *= rhs;

        return *this;
    }

    vec2<T>& operator*=(const vec2<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;

        return *this;
    }

    template<typename U>
    vec2<T>& operator/=(U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");

        x /= rhs;
        y /= rhs;

        return *this;
    }

    vec2<T>& operator/=(const vec2<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;

        return *this;
    }

#pragma endregion

#pragma region Binary

    constexpr friend vec2<T> operator+(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    template<typename U>
    constexpr friend vec2<T> operator+(const vec2<T>& lhs, U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs.x + rhs, lhs.y + rhs };
    }

    template<typename U>
    constexpr friend vec2<T> operator+(U lhs, const vec2<T>& rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs + rhs.x, lhs + rhs.y };
    }

    constexpr friend vec2<T> operator-(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    template<typename U>
    constexpr friend vec2<T> operator-(const vec2<T>& lhs, U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs.x - rhs, lhs.y - rhs };
    }

    template<typename U>
    constexpr friend vec2<T> operator-(U lhs, const vec2<T>& rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs - rhs.x, lhs - rhs.y };
    }

    constexpr friend vec2<T> operator*(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return { lhs.x * rhs.x, lhs.y * rhs.y };
    }

    template<typename U>
    constexpr friend vec2<T> operator*(const vec2<T>& lhs, U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs.x * rhs, lhs.y * rhs };
    }

    template<typename U>
    constexpr friend vec2<T> operator*(U lhs, const vec2<T>& rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs * rhs.x, lhs * rhs.y };
    }

    constexpr friend vec2<T> operator/(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return { lhs.x / rhs.x, lhs.y / rhs.y };
    }

    template<typename U>
    constexpr friend vec2<T> operator/(const vec2<T>& lhs, U rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs.x / rhs, lhs.y / rhs };
    }

    template<typename U>
    constexpr friend vec2<T> operator/(U lhs, const vec2<T>& rhs)
    {
        static_assert(std::is_arithmetic<U>::value, "should be an arithmetic type");
        return { lhs / rhs.x, lhs / rhs.y };
    }

#pragma endregion

#pragma region Boolean

    constexpr friend bool operator==(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    constexpr friend bool operator!=(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return lhs.x != rhs.x || lhs.y != rhs.y;
    }

#pragma endregion

    float length() const
    {
        return std::sqrt(x * x + y * y);
    }

    float lengthSq() const
    {
        return x * x + y * y;
    }

    float dot(const vec2<T>& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }

    float cross(const vec2<T>& rhs) const
    {
        return x * rhs.y - y * rhs.x;
    }

    vec2<T> normalize() const
    {
        return { x / length(), y / length() };
    }

    vec2<T> safeNormalize() const
    {
        if (lengthSq() < 1.0e-7)
        {
            return {};
        }
        return normalize();
    }
};

using vec2f = vec2<float>;
using vec2i = vec2<i32>;

struct Transform
{
    vec2f Position = { 0.0f, 0.0f };
    vec2f Size = { 1.0f, 1.0f };
};
