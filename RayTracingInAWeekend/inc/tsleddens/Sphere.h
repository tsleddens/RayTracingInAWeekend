#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
    class Sphere: public IRayTraceable
    {
        Point3 m_location;
        ColorCode m_color;
        float m_radius;

    public:
        Sphere(const Point3& location, float radius, const Color& color);

        [[nodiscard]] const Point3& GetLocation() const { return m_location; }
        [[nodiscard]] float GetRadius() const { return m_radius; }
        [[nodiscard]] ColorCode Intersect(const Ray& ray) const override;
    };
}