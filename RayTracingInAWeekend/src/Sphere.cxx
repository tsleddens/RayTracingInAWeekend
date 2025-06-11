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
    Vector3 radiusVector(radius);
    m_boundingBox = AABB(m_position - radiusVector, m_position + radiusVector);
}

bool Sphere::Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const
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

        if (t > FLT_MIN && range.IsInRange(t, false))
        {
            hitResult.SetIntersectionAndDistance(t, ray);

            Vector3 outwardNormal = glm::normalize(hitResult.GetIntersection() - m_position);
            hitResult.SetFaceNormal(ray, outwardNormal, m_flipNormals);
            hitResult.SetMaterial(this->GetMaterial());

            GetSphereUV(outwardNormal, hitResult.u, hitResult.v);
            return true;
        }
    }

    return false;
}
