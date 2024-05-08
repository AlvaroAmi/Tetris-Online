#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>

// Only accept arithmetic types
template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
class Vector2 {
private:
    T x, y;

public:
    constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(T x, T y) : x(x), y(y) {}

    constexpr T getX() const { return x; }
    constexpr T getY() const { return y; }

    constexpr void setX(T newX) { x = newX; }
    constexpr void setY(T newY) { y = newY; }

    // Operator overloading
    constexpr Vector2<T> operator+(const Vector2<T> &other) const {
        return Vector2<T>(x + other.x, y + other.y);
    }

    constexpr Vector2<T> operator+=(const Vector2<T> &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vector2<T> operator-(const Vector2<T> &other) const {
        return Vector2<T>(x - other.x, y - other.y);
    }

    constexpr Vector2<T> operator-=(const Vector2<T> &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vector2<T> operator*(T scalar) const {
        return Vector2<T>(x * scalar, y * scalar);
    }

    constexpr Vector2<T> operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr Vector2<T> operator/(T scalar) const {
        if (scalar != 0) {
            return Vector2<T>(x / scalar, y / scalar);
        } else {
            std::cerr << "Error: Division by zero!" << std::endl;
            return *this;
        }
    }

    constexpr Vector2<T> operator/=(T scalar) {
        if (scalar != 0) {
            x /= scalar;
            y /= scalar;
        } else {
            std::cerr << "Error: Division by zero!" << std::endl;
        }
        return *this;
    }

    constexpr bool operator==(const Vector2<T> &other) const {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Vector2<T> &other) const {
        return x != other.x || y != other.y;
    }

    constexpr T dot(const Vector2<T> &other) const {
        return x * other.x + y * other.y;
    }

    constexpr double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

#endif