#pragma once

#include "IMaterial.h"

namespace tsleddens
{
    struct IRayTraceable;
}

namespace tsleddens
{
    class HitResult
    {
        IMaterial* m_pMaterial = nullptr;

    public:
        void Initialize(const IRayTraceable& rayTraceable);

        [[nodiscard]] const IMaterial* GetMaterial() const { return m_pMaterial; }

        void SetMaterial(IMaterial* material) { m_pMaterial = material; }
    };
}
