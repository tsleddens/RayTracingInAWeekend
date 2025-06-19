#pragma once

#include <memory>

#include "../HitResult.h"
#include "../Textures/ColorTexture.h"
#include "IMaterial.h"
#include "tsleddens/ONB.h"
#include "tsleddens/PDF.h"

namespace tsleddens
{
    class Lambertian : public IMaterial
    {
        std::shared_ptr<ITexture> m_texture;

    public:
        explicit Lambertian(const std::shared_ptr<ITexture>& texture) :
            m_texture(texture)
        {
        }

        explicit Lambertian(const Color& albedo) :
            m_texture(std::make_shared<ColorTexture>(albedo))
        {
        }

        Lambertian(const float r, const float g, const float b) :
            Lambertian(std::make_shared<ColorTexture>(Color(r, g, b)))
        {
        }

        explicit Lambertian(const float rgb) :
            Lambertian(std::make_shared<ColorTexture>(Color(rgb)))
        {
        }

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, ScatterResult& scatterResult) const override
        {
            const ONB uvw(hitResult.GetNormal());

            if (Vector3 scatterDirection = uvw.Transform(RandomCosineDirection()); IsNearZero(scatterDirection))
            {
                scatterDirection = hitResult.GetNormal();
            }

            scatterResult.Attenuation = m_texture->Value(hitResult.u, hitResult.v, hitResult.GetIntersection());
            scatterResult.pPdf = std::make_shared<CosinePDF>(hitResult.GetNormal());
            scatterResult.SkipPdf = false;
            return true;
        }

        [[nodiscard]] float ScatteringPdf(const Ray& ray, const HitResult& hitResult, const Ray& scattered) const override
        {
            const float cosTheta = glm::dot(hitResult.GetNormal(), glm::normalize(scattered.GetDirection()));
            return cosTheta < 0.f ? 0.f : cosTheta / glm::pi<float>();
        }
    };
}
