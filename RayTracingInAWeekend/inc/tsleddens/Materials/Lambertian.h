#pragma once

#include <memory>

#include "../HitResult.h"
#include "../Textures/ColorTexture.h"
#include "IMaterial.h"
#include "tsleddens/ONB.h"

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

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered, float& pdf) const override
        {
            ONB uvw(hitResult.GetNormal());
            Vector3 scatterDirection = uvw.Transform(RandomCosineDirection());

            if (IsNearZero(scatterDirection))
            {
                scatterDirection = hitResult.GetNormal();
            }

            scattered = Ray(hitResult.GetIntersection(), glm::normalize(scatterDirection));
            attenuation = m_texture->Value(hitResult.u, hitResult.v, hitResult.GetIntersection());
            pdf = glm::dot(uvw.W(), scattered.GetDirection()) / glm::pi<float>();
            return true;
        }

        [[nodiscard]] float ScatteringPdf(const Ray& ray, const HitResult& hitResult, const Ray& scattered) const override
        {
            return 1.f / (2.f * glm::pi<float>());
        }
    };
}
