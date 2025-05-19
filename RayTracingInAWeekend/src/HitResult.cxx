#include "HitResult.h"

#include "IRayTraceable.h"

using namespace tsleddens;

void HitResult::Initialize(const IRayTraceable& rayTraceable)
{
    m_pMaterial = rayTraceable.GetMaterial();
}
