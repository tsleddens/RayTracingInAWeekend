#pragma once
#include "Defines.h"

namespace tsleddens
{
    class Ray;

    struct IRayTraceable
    {
        virtual ~IRayTraceable() = default;

        [[nodiscard]] virtual ColorCode Intersect(const Ray& ray) const = 0;
    };
}