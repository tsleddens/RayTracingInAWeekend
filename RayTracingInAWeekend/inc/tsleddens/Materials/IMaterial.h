#pragma once

namespace tsleddens
{
    class HitResult;
    class Ray;

    struct IMaterial
    {
        virtual ~IMaterial() = default;

        [[nodiscard]] virtual bool Scatter(const Ray&, const HitResult&, Color&, Ray&, float&) const
        {
            return false;
        }

        [[nodiscard]] virtual Color Emitted(float, float, const Point3&) const
        {
            return Color(0.f);
        }

        [[nodiscard]] virtual float ScatteringPdf(const Ray&, const HitResult&, const Ray&) const
        {
            return 0.f;
        }
    };
}
