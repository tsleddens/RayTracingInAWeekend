#pragma once
#include "ScatterResult.h"

namespace tsleddens
{
    class HitResult;
    class Ray;

    struct IMaterial
    {
        virtual ~IMaterial() = default;

        [[nodiscard]] virtual bool Scatter(const Ray&, const HitResult&, ScatterResult&) const
        {
            return false;
        }

        [[nodiscard]] virtual Color Emitted(const Ray&, const HitResult&, float, float, const Point3&) const
        {
            return Color(0.f);
        }

        [[nodiscard]] virtual float ScatteringPdf(const Ray&, const HitResult&, const Ray&) const
        {
            return 0.f;
        }
    };
}
