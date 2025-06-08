#pragma once
#include "ITexture.h"

namespace tsleddens
{
    class ColorTexture : public ITexture
    {
        Color m_albedo{};

    public:
        ColorTexture(const Color& color) :
            m_albedo(color)
        {
        }

        ColorTexture(float r, float g, float b) :
            m_albedo(Color(r, g, b))
        {
        }

        Color Value(float u, float v, const Point3& p) const override { return m_albedo; }
    };
}
