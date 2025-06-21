#pragma once

#include "HitResult.h"
#include "IRayTraceable.h"
#include "Materials/Isotropic.h"
#include "Textures/ITexture.h"

namespace tsleddens
{
class ConstantMedium : public IRayTraceable
{
    std::shared_ptr<IRayTraceable> m_boundary;
    std::shared_ptr<IMaterial>     m_phaseFunction;
    float                          m_negativeInvDensity;

public:
    ConstantMedium( const std::shared_ptr<IRayTraceable>& boundary, const float density,
                    std::shared_ptr<ITexture> texture )
    : m_boundary( boundary )
    , m_phaseFunction( std::make_shared<Isotropic>( texture ) )
    , m_negativeInvDensity( -1 / density )
    {}

    ConstantMedium( const std::shared_ptr<IRayTraceable>& boundary, const float density, const Color& albedo )
    : ConstantMedium( boundary, density, std::make_shared<ColorTexture>( albedo ) )
    {}

    [[nodiscard]] bool Intersect( const Ray& ray, HitResult& hitResult, Range<float> range ) const override
    {
        HitResult hitResult1, hitResult2;

        if ( !m_boundary->Intersect( ray, hitResult1, Range( -FLT_MAX, FLT_MAX ) ) )
        {
            return false;
        }

        if ( !m_boundary->Intersect( ray, hitResult2, Range( hitResult1.GetDistance() + 0.001f, FLT_MAX ) ) )
        {
            return false;
        }

        if ( hitResult1.GetDistance() < range.GetMin() )
            hitResult1.SetIntersection( range.GetMin() );
        if ( hitResult2.GetDistance() > range.GetMax() )
            hitResult2.SetIntersection( range.GetMax() );

        if ( hitResult1.GetDistance() >= hitResult2.GetDistance() )
        {
            return false;
        }

        if ( hitResult1.GetDistance() < 0.f )
        {
            hitResult1.SetIntersection( 0.f );
        }

        const float rayLength              = glm::length( ray.GetDirection() );
        const float distanceInsideBoundary = ( hitResult2.GetDistance() - hitResult1.GetDistance() ) * rayLength;
        const float hitDistance            = m_negativeInvDensity * std::log( RandomFloat() );

        if ( hitDistance > distanceInsideBoundary )
        {
            return false;
        }

        hitResult.SetIntersection( hitResult1.GetDistance() + hitDistance / rayLength );
        hitResult.SetIntersectionAndDistance( hitResult.GetDistance(), ray );
        hitResult.SetFaceNormal( Vector3( 1.f, 0.f, 0.f ) );
        hitResult.SetFrontFace( true );
        hitResult.SetMaterial( m_phaseFunction.get() );

        return true;
    }

    [[nodiscard]] IMaterial* GetMaterial() const override
    {
        return m_phaseFunction.get();
    }
    [[nodiscard]] bool HasFlippedNormals() const override
    {
        return false;
    }
    [[nodiscard]] const AABB& BoundingBox() const override
    {
        return m_boundary->BoundingBox();
    }
};
}  // namespace tsleddens
