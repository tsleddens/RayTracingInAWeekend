#include "Sphere.h"

#include "Ray.h"

using namespace tsleddens;

Sphere::Sphere(const Point3& location, float radius, const Color& color) :
    m_location(location),
    m_color(ColorToColorCode(color)),
    m_radius(radius)
{
}

ColorCode Sphere::Intersect(const Ray& ray) const
{
    const Point3 oc = m_location - ray.Origin;
    const float a = length2(ray.Direction);
    const float half_b = dot(oc, ray.Direction);
    const float c = length2(oc) - m_radius * m_radius;
    const float discriminant = half_b * half_b - a * c;
    if(discriminant >= 0)
    {
        return m_color;
    }

    return 0;
}
