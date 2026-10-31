#pragma once

#include <cstdint>

namespace engine {

    class Vector {
    public:
        float x{0.0f};
        float y{0.0f};

        Vector() = default;
        Vector(float x, float y);

        Vector operator+(const Vector& other) const;
        Vector operator-(const Vector& other) const;
        Vector operator*(float scalar) const;
        Vector operator/(float scalar) const;

        Vector& operator+=(const Vector& other);
        Vector& operator-=(const Vector& other);
        Vector& operator*=(float scalar);
        Vector& operator/=(float scalar);

        bool operator==(const Vector& other) const;
        bool operator!=(const Vector& other) const;

        float length() const;
        float lengthSquared() const;
        Vector normalized() const;
    };

    class Size {
    public:
        float width{0.0f};
        float height{0.0f};

        Size() = default;
        Size(float width, float height);

        bool operator==(const Size& other) const;
        bool operator!=(const Size& other) const;
    };

    namespace Math {
        void setSeed(unsigned int seed);
        int randomInt(int min, int max);
        float randomFloat(float min, float max);
        float random01();
    }

}
