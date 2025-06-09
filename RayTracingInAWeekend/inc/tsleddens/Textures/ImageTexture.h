#pragma once
#include "ITexture.h"
#include "../RtwImage.h"

namespace tsleddens
{
    class ImageTexture : public ITexture
    {
        RtwImage m_image;

    public:
        ImageTexture(const char* filename) :
            m_image(filename)
        {
        }

        Color Value(float u, float v, const Point3& p) const override
        {
            if (m_image.Height() <= 0) return Color(0.f, 1.f, 1.f);

            u = glm::clamp(u, 0.f, 1.f);
            v = 1.f - glm::clamp(v, 0.f, 1.f);

            int i = static_cast<int>(u * static_cast<float>(m_image.Width()));
            int j = static_cast<int>(v * static_cast<float>(m_image.Height()));
            const unsigned char* pixel = m_image.PixelData(i, j);

            auto colorScale = 1.f / 255.f;
            return Color(pixel[0], pixel[1], pixel[2]) * colorScale;
        }
    };
}
