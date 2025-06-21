#include "Quad.h"

#include "HitResult.h"
#include "Ray.h"

using namespace tsleddens;

bool Quad::Intersect( const Ray& ray, HitResult& hitResult, Range<float> range ) const
{
    const float denominator = glm::dot( m_normal, ray.GetDirection() );

    if ( std::fabs( denominator ) >= 1e-8f )
    {
        float t = ( m_d - glm::dot( m_normal, ray.GetOrigin() ) ) / denominator;
        if ( range.IsInRange( t, false ) )
        {
            const Point3  intersection   = ray.At( t );
            const Vector3 planarHitpoint = intersection - m_q;
            const float   alpha          = glm::dot( m_w, glm::cross( planarHitpoint, m_v ) );

            if ( const float beta = glm::dot( m_w, glm::cross( m_u, planarHitpoint ) );
                 IsInterior( alpha, beta, hitResult ) )
            {
                hitResult.SetIntersectionAndDistance( t, ray );
                hitResult.SetFaceNormal( ray, m_normal, false );
                hitResult.SetMaterial( this->GetMaterial() );
                return true;
            }
        }
    }

    return false;
}

float Quad::PdfValue( const Point3& origin, const Vector3& direction ) const
{
    HitResult hitResult;
    if ( !this->Intersect( Ray( origin, direction ), hitResult, Range<float>( 0.001f, FLT_MAX ) ) )
    {
        return 0.f;
    }

    const float distanceSquared = hitResult.GetDistance() * hitResult.GetDistance() * glm::length2( direction );
    const float cosine          = std::fabs( glm::dot( direction, hitResult.GetNormal() ) ) / glm::length2( direction );

    return distanceSquared / ( cosine * m_area );
}

Vector3 Quad::Random( const Point3& origin ) const
{
    const Vector3 p = m_q + ( RandomFloat() * m_u ) + ( RandomFloat() * m_v );
    return p - origin;
}

bool Quad::IsInterior( float a, float b, HitResult& hitResult )
{
    Range<float> unitRange( 0.f, 1.f );

    const bool isInterior = unitRange.IsInRange( a ) && unitRange.IsInRange( b );
    if ( isInterior )
    {
        hitResult.u = a;
        hitResult.v = b;
    }

    return isInterior;
}

bool Box::Intersect( const Ray& ray, HitResult& hitResult, Range<float> range ) const
{
    bool isHit = false;
    for ( UINT i = 0; i < 6; ++i )
    {
        if ( m_faces[i]->Intersect( ray, hitResult, range ) )
        {
            isHit = true;
            range.SetMax( hitResult.GetDistance() );
        }
    }

    return isHit;
}
