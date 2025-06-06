#pragma once
#include "AABB.h"
#include "Range.h"

namespace tsleddens
{
    struct IMaterial;
    class Ray;
    class HitResult;

    struct IRayTraceable
    {
        virtual ~IRayTraceable() = default;

        [[nodiscard]] virtual bool Intersect(const Ray& ray, HitResult& hitResult, Range<float>& range) const = 0;
        [[nodiscard]] virtual IMaterial* GetMaterial() const = 0;
        [[nodiscard]] virtual bool HasFlippedNormals() const = 0;
        [[nodiscard]] virtual const AABB& BoundingBox() const = 0;
    };
}
