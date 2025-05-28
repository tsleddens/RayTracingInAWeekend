#pragma once

namespace tsleddens
{
    class HitResult;
    class Ray;

    struct IMaterial
    {
        virtual ~IMaterial() = default;
        virtual [[nodiscard]] bool Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const = 0;
    };
}