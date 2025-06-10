#pragma once

#include <memory>

#include "../HitResult.h"
#include "../Textures/ColorTexture.h"
#include "IMaterial.h"

namespace tsleddens
{
    class Lambertian : public IMaterial
    {
        std::shared_ptr<ITexture> m_texture;

    public:
        Lambertian(const std::shared_ptr<ITexture>& texture) :
            m_texture(texture)
        {
        }

        Lambertian(const Color& albedo) :
            m_texture(std::make_shared<ColorTexture>(albedo))
        {
        }

        Lambertian(const float r, const float g, const float b) :
            Lambertian(std::make_shared<ColorTexture>(Color(r, g, b)))
        {
        }

        Lambertian(const float rgb) :
            Lambertian(std::make_shared<ColorTexture>(Color(rgb)))
        {
        }

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation,
                                   Ray& scattered) const override;
    };

    inline bool Lambertian::Scatter(const Ray&, const HitResult& hitResult, Color& attenuation, Ray& scattered) const
    {
        Vector3 scatterDirection = hitResult.GetNormal() + RandomUnitVector3();

        if (IsNearZero(scatterDirection))
        {
            scatterDirection = hitResult.GetNormal();
        }

        scattered = Ray(hitResult.GetIntersection(), scatterDirection);
        attenuation = m_texture->Value(hitResult.u, hitResult.v, hitResult.GetIntersection());
        return true;
    }
}
