#pragma once

#include "IMaterial.h"

namespace tsleddens
{
    class Metal : public IMaterial
    {
        Color m_albedo;

    public:
        Metal(const Color& albedo)
            : m_albedo(albedo)
        {
        }

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const override;
    };
}
