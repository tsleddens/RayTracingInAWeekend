#include "AABB.h"

#include "Ray.h"

using namespace tsleddens;

bool AABB::IsHit( const Ray& ray, Range<float> minMax ) const
{
    const Point3&  origin    = ray.GetOrigin();
    const Vector3& direction = ray.GetDirection();

    for ( int axis = 0; axis < 3; ++axis )
    {
        const Range<float>& range      = AxisRange( static_cast<EAxis>( axis ) );
        const float         reciprocal = 1.0f / direction[axis];

        float tMin = ( range.GetMin() - origin[axis] ) * reciprocal;
        float tMax = ( range.GetMax() - origin[axis] ) * reciprocal;

        if ( tMin < tMax )
        {
            if ( tMin > minMax.GetMin() )
                minMax.SetMin( tMin );
            if ( tMax < minMax.GetMax() )
                minMax.SetMax( tMax );
        }
        else
        {
            if ( tMax > minMax.GetMin() )
                minMax.SetMin( tMax );
            if ( tMin < minMax.GetMax() )
                minMax.SetMax( tMin );
        }

        if ( minMax.GetMax() <= minMax.GetMin() )
            return false;
    }
    return true;
}
