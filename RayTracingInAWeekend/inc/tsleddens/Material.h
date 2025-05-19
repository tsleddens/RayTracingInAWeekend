#pragma once

#include "IMaterial.h"

namespace tsleddens
{
    class Material : public IMaterial
    {
        Color m_color;
    public:
        Material(const Color& color)
        {
            m_color.x = color.x;
            m_color.y = color.y;
            m_color.z = color.z;
        }

        [[nodiscard]] ColorCode GetColor() const override;
    };
}
