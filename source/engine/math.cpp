#include <engine/math.hpp>
#include <cmath>
#include <cstdlib>
#include <algorithm>

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

    namespace Math {
        static uint32_t s_rngState = 2463534242u;

        void setSeed(unsigned int seed) {
            if (seed == 0) seed = 1337;
            s_rngState = seed;
        }

        static uint32_t xorshift32() {
            uint32_t x = s_rngState;
            if (x == 0) x = 123456789;
            x ^= x << 13;
            x ^= x >> 17;
            x ^= x << 5;
            s_rngState = x;
            return x;
        }

        int randomInt(int min, int max) {
            if (min > max) std::swap(min, max);
            uint32_t range = static_cast<uint32_t>(max - min + 1);
            return min + static_cast<int>(xorshift32() % range);
        }

        float randomFloat(float min, float max) {
            uint32_t val = xorshift32();
            float norm = static_cast<float>(val) / static_cast<float>(0xFFFFFFFF);
            return min + norm * (max - min);
        }

        float random01() {
            return randomFloat(0.0f, 1.0f);
        }
    }

}
