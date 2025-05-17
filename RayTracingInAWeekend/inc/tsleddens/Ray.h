#pragma once
#include "Defines.h"

namespace tsleddens
{
    class Ray
    {
    public:
        Point3 Origin;
        Vector3 Direction;

        Ray() = default;
        Ray(const Point3& origin, const Vector3& direction) :
            Origin(origin),
            Direction(direction)
        {
        }

        [[nodiscard]] Point3 At(float distance) const { return Origin + distance * Direction; }
        [[nodiscard]] Vector3 NormalizedDirection() const { return glm::normalize(Direction); }
    };
}
