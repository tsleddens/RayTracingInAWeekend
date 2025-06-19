#pragma once
#include "AABB.h"
#include "Range.h"
#include "Materials/IMaterial.h"


namespace tsleddens
{
    class Ray;
    class HitResult;

    struct IRayTraceable
    {
        virtual ~IRayTraceable() = default;

        [[nodiscard]] virtual bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const = 0;
        [[nodiscard]] virtual IMaterial* GetMaterial() const = 0;
        [[nodiscard]] virtual bool HasFlippedNormals() const = 0;
        [[nodiscard]] virtual const AABB& BoundingBox() const = 0;

        [[nodiscard]] virtual float PdfValue(const Point3&, const Vector3&) const
        {
            return 0.f;
        }

        [[nodiscard]] virtual Vector3 Random(const Point3&) const
        {
            return Vector3(1.f, 0.f, 0.f);
        }
    };
}
