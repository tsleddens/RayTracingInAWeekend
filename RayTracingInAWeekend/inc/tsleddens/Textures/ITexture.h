#pragma once

namespace tsleddens
{
    struct ITexture
    {
        virtual ~ITexture() = default;
        virtual Color Value(float u, float v, const Point3& p) const = 0;
    };
}
