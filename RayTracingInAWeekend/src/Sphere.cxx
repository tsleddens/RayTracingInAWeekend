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
    const Vector3 oc = m_position - ray.GetOrigin();
    const float a = glm::length2(ray.GetDirection());
    const float h = glm::dot(ray.GetDirection(), oc);
    const float c = glm::length2(oc) - m_radius2;

    const float discriminant = h * h - a * c;
    if (discriminant >= 0.f)
    {
        const float sqrtd = std::sqrt(discriminant);

        float root = (h - sqrtd) / a;
        if (!range.IsInRange(root))
        {
            root = (h + sqrtd) / a;
            if (!range.IsInRange(root))
            {
                return false;
            }
        }

        hitResult.SetIntersectionAndDistance(root, ray);
        Vector3 outwardNormal = (hitResult.GetIntersection() - m_position) / m_radius;
        hitResult.SetFaceNormal(ray, outwardNormal, m_flipNormals);
        hitResult.SetMaterial(m_pMaterial);
        GetSphereUV(outwardNormal, hitResult.u, hitResult.v);
        return true;
    }

    return false;
}
