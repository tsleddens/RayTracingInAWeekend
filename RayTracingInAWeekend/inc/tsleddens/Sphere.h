#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
    class Sphere: public IRayTraceable
    {
        Point3 m_position;
        IMaterial* m_pMaterial;
        float m_radius;
        float m_radius2;

    public:
        Sphere(const Point3& position, float radius, IMaterial* pMaterial);

        [[nodiscard]] const Point3& GetPosition() const { return m_position; }

        [[nodiscard]] IMaterial* GetMaterial() const override { return m_pMaterial; }

        [[nodiscard]] float GetRadius() const { return m_radius; }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, float minDistance, float maxDistance) const override;
    };
}