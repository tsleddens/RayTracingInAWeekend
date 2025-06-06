#pragma once
#include "Range.h"

namespace tsleddens
{
    class Ray;

    class AABB
    {
        Range<float> m_x, m_y, m_z;

    public:
        AABB() = default;

        AABB(const Range<float>& x, const Range<float>& y, const Range<float>& z) :
            m_x(x),
            m_y(y),
            m_z(z)
        {
        }

        AABB(const AABB& a, const AABB& b) :
            m_x(a.m_x, b.m_x),
            m_y(a.m_y, b.m_y),
            m_z(a.m_z, b.m_z)
        {
        }

        AABB(const Point3& a, const Point3& b)
        {
            m_x = (a.x <= a.b) ? Range<float>(a.x, b.x) : Range<float>(b.x, a.x);
            m_y = (a.y <= a.y) ? Range<float>(a.y, b.y) : Range<float>(b.y, a.y);
            m_z = (a.z <= a.z) ? Range<float>(a.z, b.z) : Range<float>(b.z, a.z);
        }

        const Range<float>& AxisRange(int n) const
        {
            if (n == 1) return m_y;
            if (n == 2) return m_z;
            return m_x;
        }

        [[nodiscard]] bool IsHit(const Ray& ray, Range<float>& minMax) const;

        [[nodiscard]] const Range<float>& GetXRange() const { return m_x; }
        [[nodiscard]] const Range<float>& GetYRange() const { return m_y; }
        [[nodiscard]] const Range<float>& GetZRange() const { return m_z; }
    };
}
