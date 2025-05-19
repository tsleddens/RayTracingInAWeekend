#pragma once

namespace tsleddens
{
    struct IMaterial
    {
        virtual ~IMaterial() = default;
        virtual [[nodiscard]] ColorCode GetColor() const = 0;
    };
}