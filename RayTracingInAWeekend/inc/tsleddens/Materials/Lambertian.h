#pragma once

#include "../HitResult.h"
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

    inline bool Lambertian::Scatter(const Ray&, const HitResult& hitResult, Color& attenuation, Ray& scattered) const
    {
        Vector3 scatterDirection = hitResult.GetNormal() + RandomUnitVector3();

        if (IsNearZero(scatterDirection))
        {
            scatterDirection = hitResult.GetNormal();
        }

        scattered = Ray(hitResult.GetIntersection(), scatterDirection);
        attenuation = m_albedo;
        return true;
    }
}
