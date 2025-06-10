#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
    class Quad : public IRayTraceable
    {
        Point3 m_q;

        Vector3 m_u, m_v;
        Vector3 m_w;
        Vector3 m_normal;

        float m_d;

        IMaterial* m_pMaterial;

        AABB m_boundingBox;

    public:
        Quad(const Point3& q, const Vector3& u, const Vector3& v, IMaterial* pMaterial) :
            m_q(q),
            m_u(u),
            m_v(v),
            m_pMaterial(pMaterial),
            m_boundingBox(InitBoundingBox())
        {
            Vector3 n = glm::cross(u, v);
            m_normal = glm::normalize(n);
            m_d = glm::dot(m_normal, q);
            m_w = n / glm::dot(n, n);
        }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override;
        [[nodiscard]] IMaterial* GetMaterial() const override { return m_pMaterial; }
        [[nodiscard]] bool HasFlippedNormals() const override { return false; }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }

    private:
        [[nodiscard]] AABB InitBoundingBox() const
        {
            const auto diagonal1 = AABB(m_q, m_q + m_u + m_v);
            const auto diagonal2 = AABB(m_q + m_u, m_q + m_v);
            return AABB(diagonal1, diagonal2);
        }

        static bool IsInterior(float a, float b, HitResult& hitResult);
    };
}
