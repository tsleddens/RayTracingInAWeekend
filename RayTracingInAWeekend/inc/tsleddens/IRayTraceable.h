#pragma once

namespace tsleddens
{
    struct IMaterial;
    class Ray;
    class HitResult;

    struct IRayTraceable
    {
        virtual ~IRayTraceable() = default;

        [[nodiscard]] virtual bool Intersect(const Ray& ray, HitResult& hitResult) const = 0;
        [[nodiscard]] virtual IMaterial* GetMaterial() const = 0;
    };
}