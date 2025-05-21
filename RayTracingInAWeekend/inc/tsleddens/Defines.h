#pragma once

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

inline Color ColorCodeToColor(ColorCode colorCode)
{
    constexpr float reciprocal = 1.0f / 255.0f;
    const float r = ((colorCode & 0xFF0000) >> 16) * reciprocal;
    const float g = ((colorCode & 0x00FF00) >> 8) * reciprocal;
    const float b = (colorCode & 0x0000FF) * reciprocal;
    return Color(r, g, b);
}