#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
    class Sphere: public IRayTraceable
    {
        Point3 m_location;
        IMaterial* m_pMaterial;
        float m_radius;
        float m_radiusSquared;

    public:
        Sphere(const Point3& location, float radius, IMaterial* pMaterial);

        [[nodiscard]] const Point3& GetLocation() const { return m_location; }

        [[nodiscard]] IMaterial* GetMaterial() const override { return m_pMaterial; }

        [[nodiscard]] float GetRadius() const { return m_radius; }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, float minDistance, float maxDistance) const override;
    };
}