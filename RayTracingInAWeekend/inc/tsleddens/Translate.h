#pragma once

#include "HitResult.h"
#include "IRayTraceable.h"
#include "Ray.h"

namespace tsleddens
{
    class Translate : public IRayTraceable
    {
        std::shared_ptr<IRayTraceable> m_object;
        Vector3 m_offset;
        AABB m_boundingBox;

    public:
        Translate(const std::shared_ptr<IRayTraceable>& object, const Vector3& offset) :
            m_object(object),
            m_offset(offset),
            m_boundingBox(object->BoundingBox() + offset)
        {
        }

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override
        {
            const Ray rayOffset(ray.GetOrigin() - m_offset, ray.GetDirection());

            const bool isHit = m_object->Intersect(rayOffset, hitResult, range);
            if (isHit)
            {
                hitResult.OffsetIntersection(m_offset);
            }

            return isHit;
        }

        [[nodiscard]] IMaterial* GetMaterial() const override { return m_object->GetMaterial(); }
        [[nodiscard]] bool HasFlippedNormals() const override { return m_object->HasFlippedNormals(); }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }
    };
}
