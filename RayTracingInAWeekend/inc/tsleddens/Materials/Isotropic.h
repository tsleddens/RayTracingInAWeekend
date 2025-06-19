#pragma once
#include "HitResult.h"
#include "IMaterial.h"
#include "Textures/ColorTexture.h"
#include "Textures/ITexture.h"


namespace tsleddens
{
    class Isotropic : public IMaterial
    {
        std::shared_ptr<ITexture> m_texture;

    public:
        explicit Isotropic(const std::shared_ptr<ITexture>& texture) :
            m_texture(texture)
        {
        }

        explicit Isotropic(const Color& albedo) :
            Isotropic(std::make_shared<ColorTexture>(albedo))
        {
        }

        Isotropic(const float r, const float g, const float b) :
            Isotropic(Color(r, g, b))
        {
        }

        explicit Isotropic(const float rgb) :
            Isotropic(Color(rgb))
        {
        }

        ~Isotropic() override = default;

        [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered, float& pdf) const override
        {
            scattered = Ray(hitResult.GetIntersection(), RandomUnitVector3());
            attenuation = m_texture->Value(hitResult.u, hitResult.v, hitResult.GetIntersection());
            pdf = 1.f / (4.f * glm::pi<float>());
            return true;
        }

        [[nodiscard]] float ScatteringPdf(const Ray&, const HitResult&, const Ray&) const override
        {
            return 1.f / (4.f * glm::pi<float>());
        }
    };
}
