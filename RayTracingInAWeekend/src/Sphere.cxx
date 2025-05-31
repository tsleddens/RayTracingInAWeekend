#include "Sphere.h"

#include "HitResult.h"
#include "Ray.h"

using namespace tsleddens;

Sphere::Sphere(const Point3& position, float radius, IMaterial* pMaterial, bool flippedNormals) :
    m_position(position),
    m_pMaterial(pMaterial),
    m_radius(radius),
    m_radius2(radius * radius),
    m_flipNormals(flippedNormals)
{
}

bool Sphere::Intersect(const Ray& ray, HitResult& hitResult, float minDistance, float maxDistance) const
{
    Vector3 diff = m_position - ray.GetOrigin();
    float tca = glm::dot(diff, ray.GetDirection());
    float distance2 = glm::dot(diff, diff) - tca * tca;

    if (distance2 <= m_radius2)
    {
        float thc = std::sqrt(m_radius2 - distance2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        float t = (t0 > 0.f) ? t0 : (t1 > 0.f) ? t1 : FLT_MIN;

        if (t > FLT_MIN && t > minDistance && t < maxDistance)
        {
            hitResult.SetDistance(t, ray);
            hitResult.SetFaceNormal(ray, glm::normalize(hitResult.GetIntersection() - m_position), m_flipNormals);
            hitResult.SetMaterial(this->GetMaterial());
            return true;
        }
    }

    return false;
}
