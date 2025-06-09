#pragma once
#include "ITexture.h"
#include "../Perlin.h"

namespace tsleddens
{
    class NoiseTexture : public ITexture
    {
        Perlin m_perlin{};

    public:
        NoiseTexture() = default;

        Color Value(float u, float v, const Point3& p) const override
        {
            return Color(1.f) * m_perlin.Noise(p);
        }
    };
}
