#pragma once

#include "HitResult.h"
#include "IRayTraceable.h"
#include "Ray.h"

namespace tsleddens
{
    class RotateY : public IRayTraceable
    {
        std::shared_ptr<IRayTraceable> m_object;
        float m_angleRadians;
        float m_sinTheta, m_cosTheta;
        AABB m_boundingBox;

    public:
        RotateY(const std::shared_ptr<IRayTraceable>& object, float angleDegrees) :
            m_object(object),
            m_angleRadians(glm::radians(angleDegrees)),
            m_sinTheta(std::sin(m_angleRadians)),
            m_cosTheta(std::cos(m_angleRadians)),
            m_boundingBox(object->BoundingBox())
        {
            Point3 min(FLT_MAX);
            Point3 max(-FLT_MAX);

            for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
            {
                const float fI = static_cast<float>(i);
                const float fJ = static_cast<float>(j);
                const float fK = static_cast<float>(k);

                const float x = fI * m_boundingBox.Max().x + (1.f - fI) * m_boundingBox.Min().x;
                const float y = fJ * m_boundingBox.Max().y + (1.f - fJ) * m_boundingBox.Min().y;
                const float z = fK * m_boundingBox.Max().z + (1.f - fK) * m_boundingBox.Min().z;

                const float newX = m_cosTheta * x + m_sinTheta * z;
                const float newZ = -m_sinTheta * x + m_cosTheta * z;

                Vector3 tester(newX, y, newZ);

                for (int c = 0; c < 3; ++c)
                {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }

            m_boundingBox = AABB(min, max);
        }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override
        {
            Point3 origin(
                (m_cosTheta * ray.GetOrigin().x) - (m_sinTheta * ray.GetOrigin().z),
                ray.GetOrigin().y,
                (m_sinTheta * ray.GetOrigin().x) + (m_cosTheta * ray.GetOrigin().z));

            Vector3 direction(
                (m_cosTheta * ray.GetDirection().x) - (m_sinTheta * ray.GetDirection().z),
                ray.GetDirection().y,
                (m_sinTheta * ray.GetDirection().x) + (m_cosTheta * ray.GetDirection().z));

            Ray rotatedRay(origin, direction);

            const bool isHit = m_object->Intersect(rotatedRay, hitResult, range);
            if (isHit)
            {
                hitResult.SetIntersection(Point3(
                    (m_cosTheta * hitResult.GetIntersection().x) + (m_sinTheta * hitResult.GetIntersection().z),
                    hitResult.GetIntersection().y,
                    (-m_sinTheta * hitResult.GetIntersection().x) + (m_cosTheta * hitResult.GetIntersection().z)));

                hitResult.SetFaceNormal(Point3(
                    (m_cosTheta * hitResult.GetNormal().x) + (m_sinTheta * hitResult.GetNormal().z),
                    hitResult.GetNormal().y,
                    (-m_sinTheta * hitResult.GetNormal().x) + (m_cosTheta * hitResult.GetNormal().z)));
            }

            return isHit;
        }

        [[nodiscard]] IMaterial* GetMaterial() const override { return m_object->GetMaterial(); }
        [[nodiscard]] bool HasFlippedNormals() const override { return m_object->HasFlippedNormals(); }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }
    };
}
