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
        [[nodiscard]] const Point3& GetIntersection() const { return m_intersection; }
        [[nodiscard]] const Vector3& GetNormal() const { return m_normal; }
        [[nodiscard]] float GetDistance() const { return m_distance; }
        [[nodiscard]] bool IsFrontFace() const { return m_frontFace; }

        void SetIntersection(const Point3& intersection) { m_intersection = Point3(intersection); }
        void SetMaterial(IMaterial* material) { m_pMaterial = material; }
        void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal);
        void SetDistance(float distance) { m_distance = distance; }
    };

    inline void HitResult::SetFaceNormal(const Ray& ray, const Vector3& outwardNormal)
    {
        m_frontFace = glm::dot(ray.GetDirection(), outwardNormal) < 0;
        m_normal = m_frontFace ? outwardNormal : -outwardNormal;
    }
}
