#pragma once
#include "Defines.h"
#include "Ray.h"

namespace tsleddens
{
    struct IMaterial;
    struct IRayTraceable;

    class HitResult
    {
        IMaterial* m_pMaterial = nullptr;
        Vector3 m_normal = {};
        Point3 m_intersection = {};
        bool m_frontFace = false;
        float m_distance = FLT_MAX;

    public:
        float u, v;

        [[nodiscard]] const Point3& GetIntersection() const { return m_intersection; }
        [[nodiscard]] const Vector3& GetNormal() const { return m_normal; }
        [[nodiscard]] float GetDistance() const { return m_distance; }
        [[nodiscard]] bool IsFrontFace() const { return m_frontFace; }
        [[nodiscard]] IMaterial* GetMaterial() const { return m_pMaterial; }

        void SetMaterial(IMaterial* material) { m_pMaterial = material; }
        void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal, bool flipped);
        void SetDistance(float distance, const Ray& ray);
        void OffsetIntersection(const Vector3& offset) { m_intersection += offset; }

        void SetIntersection(const Vector3& intersection) { m_intersection = intersection; }
        void SetFaceNormal(const Vector3& faceNormal) { m_normal = faceNormal; }
    };

    inline void HitResult::SetFaceNormal(const Ray& ray, const Vector3& outwardNormal, bool flipped)
    {
        m_frontFace = glm::dot(ray.GetDirection(), outwardNormal) < 0.f;
        m_frontFace = flipped ? !m_frontFace : m_frontFace;
        m_normal = m_frontFace ? outwardNormal : -outwardNormal;
    }

    inline void HitResult::SetDistance(float distance, const Ray& ray)
    {
        m_distance = distance;
        m_intersection = ray.At(distance);
    }
}
