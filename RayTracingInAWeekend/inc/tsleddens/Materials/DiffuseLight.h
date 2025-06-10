#pragma once

#include "IMaterial.h"
#include "../Textures/ColorTexture.h"
#include "../Textures/ITexture.h"

namespace tsleddens
{
    class DiffuseLight : public IMaterial
    {
        std::shared_ptr<ITexture> m_texture;

    public:
        DiffuseLight(const std::shared_ptr<ITexture>& texture) :
            m_texture(texture)
        {
        }

        DiffuseLight(const Color emit) :
            DiffuseLight(std::make_shared<ColorTexture>(emit))
        {
        }

        DiffuseLight(const float r, const float g, const float b) :
            DiffuseLight(Color(r, g, b))
        {
        }

        DiffuseLight(const float rgb) :
            DiffuseLight(Color(rgb))
        {
        }

        ~DiffuseLight() override = default;

        [[nodiscard]] Color Emitted(float u, float v, const Point3& p) const override
        {
            return m_texture->Value(u, v, p);
        }
    };
}
