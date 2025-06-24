#pragma once

#include "../HitResult.h"
#include "IMaterial.h"

namespace tsleddens
{
class Metal : public IMaterial
{
    Color m_albedo;
    float m_fuzz;

public:
    Metal( const Color& albedo, const float fuzz )
    : m_albedo( albedo )
    , m_fuzz( fuzz < 1.0f ? fuzz : 1.0f )
    {}

    [[nodiscard]] bool Scatter( const Ray& ray, const HitResult& hitResult, ScatterResult& scatterResult,
                                const UINT baseIndex ) const override
    {
        const Vector3 reflected        = glm::reflect( ray.GetDirection(), hitResult.GetNormal() );
        const Vector3 scatterDirection = reflected + ( m_fuzz * RandomUnitVector3() );

        scatterResult.Attenuation = m_albedo;
        scatterResult.pPdf        = nullptr;
        scatterResult.SkipPdf     = true;
        scatterResult.SkipPdfRay  = Ray( hitResult.GetIntersection(), scatterDirection );
        return true;
    }

    [[nodiscard]] float GetFuzz() const
    {
        return m_fuzz;
    }

    void SetFuzz( const float fuzz )
    {
        m_fuzz = fuzz < 1.0f ? fuzz : 1.0f;
    }
};
}  // namespace tsleddens
