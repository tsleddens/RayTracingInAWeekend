#pragma once

#include "../HitResult.h"
#include "IMaterial.h"

namespace tsleddens
{
    class Metal : public IMaterial
    {
        Color m_albedo;
        float m_fuzz;

    public:
        Metal(const Color& albedo, float fuzz) :
            m_albedo(albedo),
            m_fuzz(fuzz < 1.0f ? fuzz : 1.0f)
        {
        }

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const override;
        [[nodiscard]] float GetFuzz() const { return m_fuzz; }

        void SetFuzz(float fuzz) { m_fuzz = fuzz < 1.0f ? fuzz : 1.0f; }
    };
}

inline bool tsleddens::Metal::Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const
{
    Vector3 reflected = glm::reflect(ray.GetDirection(), hitResult.GetNormal());
    Vector3 scatterDirection = reflected + (m_fuzz * RandomUnitVector3());
    scattered = Ray(hitResult.GetIntersection(), scatterDirection);
    attenuation = m_albedo;
    return (glm::dot(scattered.GetDirection(), hitResult.GetNormal()) > 0.f);
}

