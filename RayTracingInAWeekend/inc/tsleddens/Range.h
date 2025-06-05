#pragma once
#include "Defines.h"

namespace tsleddens
{
    template <typename T>
    class Range
    {
        T m_min;
        T m_max;

    public:
        Range(T min, T max)
            : m_min(min), m_max(max)
        {
        }

        [[nodiscard]] const T& GetMin() const { return m_min; }
        [[nodiscard]] const T& GetMax() const { return m_max; }

        void SetMin(const T& min) { m_min = min; }
        void SetMax(const T& max) { m_max = max; }

        [[nodiscard]] bool IsInRange(const T& value, bool inclusive = true) const
        {
            if (inclusive)
            {
                return (value >= m_min && value <= m_max);
            }

            return (value > m_min && value < m_max);
        }
    };
}
