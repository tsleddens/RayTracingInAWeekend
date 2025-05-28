#pragma once

#define GLM_FORCE_INLINE
#define GLM_FORCE_SIMD_AVX2
#define GLM_FORCE_ALIGNED

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using Vector3 = glm::vec3;
using Point3 = glm::vec3;
using Color = glm::vec3;
using Pixel = glm::vec2;
using ColorCode = unsigned int;

inline float LinearToGamma(float linear)
{
    constexpr float gamma = 1.f / 2.2f;
    if (linear > 0.f)
    {
        return std::pow(linear, gamma); // Gamma correction
    }

    return 0.f;
}

inline ColorCode ColorToColorCode(const Color& color)
{
    const auto r = static_cast<unsigned int>(LinearToGamma(color.x) * 255.999f) & 0xff;
    const auto g = static_cast<unsigned int>(LinearToGamma(color.y) * 255.999f) & 0xff;
    const auto b = static_cast<unsigned int>(LinearToGamma(color.z) * 255.999f) & 0xff;
    return (r << 16) + (g << 8) + b;
}

inline float RandomFloat()
{
    return std::rand() / (static_cast<float>(RAND_MAX) + 1.f);
}

inline float RandomFloat(float min, float max)
{
    return min + (max - min) * RandomFloat();
}

inline Vector3 RandomVector3(float min, float max)
{
    return Vector3(
        RandomFloat(min, max),
        RandomFloat(min, max),
        RandomFloat(min, max)
    );
}

inline Vector3 RandomUnitVector3()
{
    while (true)
    {
        Vector3 p = RandomVector3(-1.f, 1.f);
        float lengthSquared = glm::length2(p);
        if (lengthSquared >= 1.f || lengthSquared < 0.00001f)
            continue; // Skip points that are outside the unit sphere or too close to the origin.

        return glm::normalize(p);
    }
}

inline Vector3 RandomBounce(const Vector3& normal)
{
    Vector3 randomDirection = RandomUnitVector3();
    if (glm::dot(randomDirection, normal) < 0.f)
    {
        randomDirection = -randomDirection; // Ensure the random direction is in the hemisphere defined by the normal.
    }
    return randomDirection;
}