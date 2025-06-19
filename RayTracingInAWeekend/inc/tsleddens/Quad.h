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
        float m_area;

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

            m_area = glm::length(n);
        }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override;
        [[nodiscard]] IMaterial* GetMaterial() const override { return m_pMaterial; }
        [[nodiscard]] bool HasFlippedNormals() const override { return false; }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }

        [[nodiscard]] float PdfValue(const Point3&, const Vector3&) const override;
        [[nodiscard]] Vector3 Random(const Point3&) const override;

    private:
        [[nodiscard]] AABB InitBoundingBox() const
        {
            const auto diagonal1 = AABB(m_q, m_q + m_u + m_v);
            const auto diagonal2 = AABB(m_q + m_u, m_q + m_v);
            return AABB(diagonal1, diagonal2);
        }

        static bool IsInterior(float a, float b, HitResult& hitResult);
    };

    class Box : public IRayTraceable
    {
        std::shared_ptr<Quad> m_faces[6];
        IMaterial* m_pMaterial;
        AABB m_boundingBox{};

    public:
        Box(const Point3& a, const Point3& b, IMaterial* pMaterial) :
            m_pMaterial(pMaterial)
        {
            const Point3 min(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
            const Point3 max(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

            const Vector3 dx(max.x - min.x, 0.f, 0.f);
            const Vector3 dy(0.f, max.y - min.y, 0.f);
            const Vector3 dz(0.f, 0.f, max.z - min.z);

            m_faces[0] = std::make_shared<Quad>(Point3(min.x, min.y, max.z), dx, dy, pMaterial);
            m_faces[1] = std::make_shared<Quad>(Point3(max.x, min.y, max.z), -dz, dy, pMaterial);
            m_faces[2] = std::make_shared<Quad>(Point3(max.x, min.y, min.z), -dx, dy, pMaterial);
            m_faces[3] = std::make_shared<Quad>(Point3(min.x, min.y, min.z), dz, dy, pMaterial);
            m_faces[4] = std::make_shared<Quad>(Point3(min.x, max.y, max.z), dx, -dz, pMaterial);
            m_faces[5] = std::make_shared<Quad>(Point3(min.x, min.y, min.z), dx, dz, pMaterial);

            for (int i = 0; i < 6; ++i)
            {
                m_boundingBox = AABB(m_boundingBox, m_faces[i]->BoundingBox());
            }
        }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override;

        [[nodiscard]] IMaterial* GetMaterial() const override { return m_pMaterial; }
        [[nodiscard]] bool HasFlippedNormals() const override { return false; }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }
    };
}
