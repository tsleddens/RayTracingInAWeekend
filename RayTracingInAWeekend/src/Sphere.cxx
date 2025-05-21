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

bool Sphere::Intersect(const Ray& ray, HitResult& hitResult, float minDistance, float maxDistance) const
{
    const Point3 c = m_location - ray.GetOrigin();
    float t = glm::dot(c, ray.GetDirection());
    if (t >= 0.f)
    {
        const Vector3 q = c - t * ray.GetDirection();
        float p2 = glm::dot(q, q);

        if (p2 <= m_radiusSquared)
        {
            t -= std::sqrt(m_radiusSquared - p2);

            if (t > minDistance && t < maxDistance)
            {
                const Point3 intersectionPoint = ray.GetOrigin() + ray.GetDirection() * t;
                hitResult.SetIntersection(intersectionPoint);
                const Vector3 outwardNormal = glm::normalize(intersectionPoint - m_location);
                hitResult.SetFaceNormal(ray, outwardNormal);
                hitResult.SetMaterial(this->GetMaterial());
                hitResult.SetDistance(t);
                return true;
            }
        }
    }

    return false;
}
