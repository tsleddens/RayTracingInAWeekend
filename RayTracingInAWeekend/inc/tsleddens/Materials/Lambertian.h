#pragma once

#include "IMaterial.h"

namespace tsleddens
{
    class Lambertian : public IMaterial
    {
        Color m_albedo;

    public:
        Lambertian(const Color& albedo)
            : m_albedo(albedo)
        {
        }

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const override;
    };
}
