#pragma once

#include <cstdint>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief 2D vector class supporting floating-point coordinates and arithmetic operations.
     */
    class Vector {
    public:
        float x{0.0f}; ///< X coordinate component.
        float y{0.0f}; ///< Y coordinate component.

        /** @brief Default constructor initializing vector to (0, 0). */
        Vector() = default;
        
        /** 
         * @brief Constructs a vector with specified X and Y coordinates.
         * @param x X component.
         * @param y Y component.
         */
        Vector(float x, float y);

        /** @brief Addition operator. @param other Vector to add. @return Resulting Vector sum. */
        Vector operator+(const Vector& other) const;
        
        /** @brief Subtraction operator. @param other Vector to subtract. @return Resulting Vector difference. */
        Vector operator-(const Vector& other) const;
        
        /** @brief Scalar multiplication operator. @param scalar Scaling factor. @return Scaled Vector. */
        Vector operator*(float scalar) const;
        
        /** @brief Scalar division operator. @param scalar Division factor. @return Scaled Vector. */
        Vector operator/(float scalar) const;

        /** @brief Compound addition assignment operator. @param other Vector to add. @return Reference to self. */
        Vector& operator+=(const Vector& other);
        
        /** @brief Compound subtraction assignment operator. @param other Vector to subtract. @return Reference to self. */
        Vector& operator-=(const Vector& other);
        
        /** @brief Compound scalar multiplication assignment operator. @param scalar Scaling factor. @return Reference to self. */
        Vector& operator*=(float scalar);
        
        /** @brief Compound scalar division assignment operator. @param scalar Division factor. @return Reference to self. */
        Vector& operator/=(float scalar);

        /** @brief Equality comparison operator. @param other Vector to compare. @return True if equal. */
        bool operator==(const Vector& other) const;
        
        /** @brief Inequality comparison operator. @param other Vector to compare. @return True if not equal. */
        bool operator!=(const Vector& other) const;

        /** @brief Calculates the Euclidean magnitude (length) of the vector. @return Magnitude value. */
        float length() const;
        
        /** @brief Calculates the squared Euclidean magnitude of the vector. @return Squared magnitude value. */
        float lengthSquared() const;
        
        /** @brief Returns a normalized unit vector in the same direction. @return Normalized Vector. */
        Vector normalized() const;
    };

    /**
     * @brief 2D size dimensions class (width and height).
     */
    class Size {
    public:
        float width{0.0f};  ///< Width dimension in pixels.
        float height{0.0f}; ///< Height dimension in pixels.

        /** @brief Default constructor initializing size to (0, 0). */
        Size() = default;
        
        /** 
         * @brief Constructs a size with specified width and height.
         * @param width Width value.
         * @param height Height value.
         */
        Size(float width, float height);

        /** @brief Equality comparison operator. @param other Size to compare. @return True if equal. */
        bool operator==(const Size& other) const;
        
        /** @brief Inequality comparison operator. @param other Size to compare. @return True if not equal. */
        bool operator!=(const Size& other) const;
    };

    /**
     * @brief Utility namespace for random number generation and mathematical operations.
     */
    namespace Math {
        /** @brief Sets the pseudo-random number generator seed. @param seed Seed value. */
        void setSeed(unsigned int seed);
        
        /** @brief Generates a random integer within a range [min, max]. @param min Minimum value (inclusive). @param max Maximum value (inclusive). @return Random integer. */
        int randomInt(int min, int max);
        
        /** @brief Generates a random floating-point number within a range [min, max]. @param min Minimum value (inclusive). @param max Maximum value (inclusive). @return Random float. */
        float randomFloat(float min, float max);
        
        /** @brief Generates a random floating-point number between 0.0 and 1.0. @return Random float in [0, 1]. */
        float random01();
    }

}
