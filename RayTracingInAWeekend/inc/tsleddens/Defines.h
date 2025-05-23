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


inline ColorCode ColorToColorCode(const Color& color)
{
    const auto r = static_cast<unsigned int>(color.x * 255.999f) & 0xff;
    const auto g = static_cast<unsigned int>(color.y * 255.999f) & 0xff;
    const auto b = static_cast<unsigned int>(color.z * 255.999f) & 0xff;
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