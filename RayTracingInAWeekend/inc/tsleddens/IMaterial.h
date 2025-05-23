#pragma once

namespace tsleddens
{
    struct IMaterial
    {
        virtual ~IMaterial() = default;
        virtual [[nodiscard]] ColorCode GetColorCode() const = 0;
        virtual [[nodiscard]] Color GetColor() const = 0;
    };
}