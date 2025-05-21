#pragma once
#include "Defines.h"

namespace tsleddens
{
    class Ray
    {
        Point3 m_origin;
        Vector3 m_direction;
    public:

        Ray() = default;
        Ray(const Point3& origin, const Vector3& direction) :
            m_origin(origin),
            m_direction(glm::normalize(direction))
        {
        }

        void SetOrigin(const Point3& origin) { m_origin = Point3(origin); }
        void SetDirection(const Vector3& direction) { m_direction = glm::normalize(direction); }

        [[nodiscard]] Point3 At(float distance) const { return m_origin + (distance * m_direction); }
        [[nodiscard]] const Vector3& GetDirection() const { return m_direction; }
        [[nodiscard]] const Point3& GetOrigin() const { return m_origin; }
    };
}
