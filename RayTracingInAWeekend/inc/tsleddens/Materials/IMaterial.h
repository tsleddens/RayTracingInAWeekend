#pragma once

namespace tsleddens
{
    class HitResult;
    class Ray;

    struct IMaterial
    {
        virtual ~IMaterial() = default;

        virtual [[nodiscard]] bool Scatter(const Ray&, const HitResult&, Color&, Ray&) const
        {
            return false;
        }

        virtual [[nodiscard]] Color Emitted(float, float, const Point3&) const
        {
            return Color(0.f);
        }
    };
}
