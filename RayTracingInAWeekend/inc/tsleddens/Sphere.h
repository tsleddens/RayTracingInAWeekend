#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
class Sphere : public IRayTraceable
{
    AABB       m_boundingBox;
    Point3     m_position;
    IMaterial* m_pMaterial;
    float      m_radius;
    float      m_radius2;
    bool       m_flipNormals;

public:
    Sphere( const Point3& position, float radius, IMaterial* pMaterial, bool flippedNormals = false );

    [[nodiscard]] const Point3& GetPosition() const
    {
        return m_position;
    }

    [[nodiscard]] IMaterial* GetMaterial() const override
    {
        return m_pMaterial;
    }

    [[nodiscard]] float GetRadius() const
    {
        return m_radius;
    }

    [[nodiscard]] bool Intersect( const Ray& ray, HitResult& hitResult, Range<float> range ) const override;

    [[nodiscard]] bool HasFlippedNormals() const override
    {
        return m_flipNormals;
    }

    [[nodiscard]] const AABB& BoundingBox() const override
    {
        return m_boundingBox;
    }

    static void GetSphereUV( const Point3& p, float& u, float& v )
    {
        float theta = std::acos( -p.y );
        float phi   = std::atan2( -p.z, p.x ) + PI;

        u = phi / ( 2.f * PI );
        v = theta / PI;
    }

    [[nodiscard]] float   PdfValue( const Point3&, const Vector3& ) const override;
    [[nodiscard]] Vector3 Random( const Point3& ) const override;

private:
    static Vector3 RandomToSphere( float radius2, float distance2 );
};
}  // namespace tsleddens
