#include <engine/math.hpp>
#include <cmath>

namespace engine {

    Vector::Vector(float x, float y) : x(x), y(y) {}

    Vector Vector::operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }

    Vector Vector::operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y);
    }

    Vector Vector::operator*(float scalar) const {
        return Vector(x * scalar, y * scalar);
    }

    Vector Vector::operator/(float scalar) const {
        return Vector(x / scalar, y / scalar);
    }

    Vector& Vector::operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& Vector::operator-=(const Vector& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector& Vector::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector& Vector::operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool Vector::operator==(const Vector& other) const {
        return x == other.x && y == other.y;
    }

    bool Vector::operator!=(const Vector& other) const {
        return !(*this == other);
    }

    float Vector::length() const {
        return std::sqrt(lengthSquared());
    }

    float Vector::lengthSquared() const {
        return x * x + y * y;
    }

    Vector Vector::normalized() const {
        float len = length();
        if (len == 0.0f) return Vector(0.0f, 0.0f);
        return *this / len;
    }

    Size::Size(float width, float height) : width(width), height(height) {}

    bool Size::operator==(const Size& other) const {
        return width == other.width && height == other.height;
    }

    bool Size::operator!=(const Size& other) const {
        return !(*this == other);
    }

}