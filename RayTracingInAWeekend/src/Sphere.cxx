#include "Sphere.h"

#include "HitResult.h"
#include "ONB.h"
#include "Ray.h"

using namespace tsleddens;

Sphere::Sphere( const Point3& position, float radius, IMaterial* pMaterial, bool flippedNormals )
: m_position( position )
, m_pMaterial( pMaterial )
, m_radius( radius )
, m_radius2( radius * radius )
, m_flipNormals( flippedNormals )
{
    Vector3 radiusVector( radius );
    m_boundingBox = AABB( m_position - radiusVector, m_position + radiusVector );
}

bool Sphere::Intersect( const Ray& ray, HitResult& hitResult, Range<float> range ) const
{
    const Vector3 oc = m_position - ray.GetOrigin();
    const float   a  = glm::length2( ray.GetDirection() );
    const float   h  = glm::dot( ray.GetDirection(), oc );
    const float   c  = glm::length2( oc ) - m_radius2;

    const float discriminant = h * h - a * c;
    if ( discriminant >= 0.f )
    {
        const float sqrtd = std::sqrt( discriminant );

        float root = ( h - sqrtd ) / a;
        if ( !range.IsInRange( root ) )
        {
            root = ( h + sqrtd ) / a;
            if ( !range.IsInRange( root ) )
            {
                return false;
            }
        }

        hitResult.SetIntersectionAndDistance( root, ray );
        Vector3 outwardNormal = ( hitResult.GetIntersection() - m_position ) / m_radius;
        hitResult.SetFaceNormal( ray, outwardNormal, m_flipNormals );
        hitResult.SetMaterial( m_pMaterial );
        GetSphereUV( outwardNormal, hitResult.u, hitResult.v );
        return true;
    }

    return false;
}

float Sphere::PdfValue( const Point3& origin, const Vector3& direction ) const
{
    if ( HitResult hitResult; !this->Intersect( Ray( origin, direction ), hitResult, Range<float>( 0.001f, FLT_MAX ) ) )
    {
        return 0.f;
    }

    const float distance2   = glm::length2( m_position - origin );
    const float cosThetaMax = std::sqrt( 1.f - m_radius2 / distance2 );
    const float solidAngle  = 2.f * glm::pi<float>() * ( 1.f - cosThetaMax );
    return 1.f / solidAngle;
}

Vector3 Sphere::Random( const Point3& origin ) const
{
    const Vector3 direction = m_position - origin;
    const float   distance2 = glm::length2( direction );
    const ONB     uvw( direction );
    return uvw.Transform( RandomToSphere( m_radius2, distance2 ) );
}

Vector3 Sphere::RandomToSphere( const float radius2, const float distance2 )
{
    const float r1 = RandomFloat();
    const float r2 = RandomFloat();
    const float z  = 1.f + r2 * ( std::sqrt( 1.f - radius2 / distance2 ) - 1.f );

    const float phi = 2.f * glm::pi<float>() * r1;
    const float x   = std::cos( phi ) * std::sqrt( 1.f - z * z );
    const float y   = std::sin( phi ) * std::sqrt( 1.f - z * z );

    return Vector3( x, y, z );
}
