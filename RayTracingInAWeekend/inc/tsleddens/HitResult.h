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
    public:
        [[nodiscard]] const Point3& GetIntersection() const { return m_intersection; }
        [[nodiscard]] const IMaterial* GetMaterial() const { return m_pMaterial; }
        [[nodiscard]] const Vector3& GetNormal() const { return m_normal; }
        [[nodiscard]] bool IsFrontFace() const { return m_frontFace; }

        void SetIntersection(const Point3& intersection) { m_intersection = Point3(intersection); }
        void SetMaterial(IMaterial* material) { m_pMaterial = material; }
        void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal)
        {
            m_frontFace = glm::dot(ray.GetDirection(), outwardNormal) < 0;
            m_normal = m_frontFace ? outwardNormal : -outwardNormal;
        }
    };
}
