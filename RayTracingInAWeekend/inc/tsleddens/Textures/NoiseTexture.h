#pragma once
#include "ITexture.h"
#include "../Perlin.h"

namespace tsleddens
{
    class NoiseTexture : public ITexture
    {
        Perlin m_perlin{};

        float m_scale;

    public:
        NoiseTexture(float scale = 1.f) :
            m_scale(scale)
        {
        }

        Color Value(float u, float v, const Point3& p) const override
        {
            return Color(1.f) * m_perlin.Turbulence(p, 7.f);
        }
    };
}
