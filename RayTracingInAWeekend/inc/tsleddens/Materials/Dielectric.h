#pragma once

#include "../HitResult.h"
#include "IMaterial.h"

namespace tsleddens
{
    class Dielectric : public IMaterial
    {
        float m_refractionIndex;

    public:
        Dielectric(float refractionIndex) :
            m_refractionIndex(refractionIndex)
        {
        }

        [[nodiscard]] float GetRefractionIndex() const { return m_refractionIndex; }

        void SetRefractionIndex(float refractionIndex) { m_refractionIndex = refractionIndex; }

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered, float& pdf) const override;

    private:

        static float Reflectance(float cosine, float refractionIndex);
    };

    inline bool Dielectric::Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered, float& pdf) const
    {
        attenuation = Color(1.f); // Dielectric materials do not absorb light
        const float refractionIndex = hitResult.IsFrontFace() ? (1.0f / m_refractionIndex) : m_refractionIndex;

        const float cosTheta = glm::clamp(glm::dot(-ray.GetDirection(), hitResult.GetNormal()), -1.0f, 1.0f);
        const float sinTheta = glm::sqrt(glm::max(0.0f, 1.0f - cosTheta * cosTheta));

        bool cannotRefract = refractionIndex * sinTheta > 1.0f;
        Vector3 direction;
        if (cannotRefract || Reflectance(cosTheta, refractionIndex) > RandomFloat())
        {
            direction = glm::reflect(ray.GetDirection(), hitResult.GetNormal());
        }
        else
        {
            direction = glm::refract(ray.GetDirection(), hitResult.GetNormal(), refractionIndex);
        }

        if (!std::isfinite(direction.x) || glm::length(direction) < EPSILON) return false;

        scattered = Ray(hitResult.GetIntersection() + direction * EPSILON, direction);


        return true;
    }

    inline float Dielectric::Reflectance(float cosine, float refractionIndex)
    {
        // Use Schlick's approximation for reflectance
        float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * glm::pow(1.0f - cosine, 5.0f);
    }
}