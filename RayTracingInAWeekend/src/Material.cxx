#include "Material.h"

using namespace tsleddens;

ColorCode Material::GetColor() const
{
    return ColorToColorCode(m_color);
}
