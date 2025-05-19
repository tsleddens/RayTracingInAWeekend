#include "Sphere.h"

#include "HitResult.h"
#include "Ray.h"

using namespace tsleddens;

Sphere::Sphere(const Point3& location, float radius, IMaterial* pMaterial) :
    m_location(location),
    m_pMaterial(pMaterial),
    m_radius(radius),
    m_radiusSquared(radius * radius)
{
}

bool Sphere::Intersect(const Ray& ray, HitResult& hitResult) const
{
    const Point3 oc = m_location - ray.Origin;
    const float a = length2(ray.Direction);
    const float half_b = dot(oc, ray.Direction);
    const float c = length2(oc) - m_radiusSquared;
    const float discriminant = half_b * half_b - a * c;

    if(discriminant >= 0)
    {
        hitResult.Initialize(*this);
        return true;
    }

    return false;
}
