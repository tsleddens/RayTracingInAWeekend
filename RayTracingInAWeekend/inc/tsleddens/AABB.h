#pragma once
#include "Defines.h"
#include "Range.h"

namespace tsleddens
{
class Ray;

class AABB
{
    Vector3 m_min, m_max;

public:
    AABB() = default;

    AABB( const AABB& a, const AABB& b )
    : m_min( glm::min( a.m_min, b.m_min ) )
    , m_max( glm::max( a.m_max, b.m_max ) )
    {}

    AABB( const Point3& a, const Point3& b )
    : m_min( glm::min( a.x, b.x ), glm::min( a.y, b.y ), glm::min( a.z, b.z ) )
    , m_max( glm::max( a.x, b.x ), glm::max( a.y, b.y ), glm::max( a.z, b.z ) )
    {
        PadToMinimums();
    }

    [[nodiscard]] Range<float> AxisRange( const EAxis n ) const
    {
        if ( n == EAxis::Y )
            return { m_min.y, m_max.y };
        if ( n == EAxis::Z )
            return { m_min.z, m_max.z };
        return { m_min.x, m_max.x };
    }

    [[nodiscard]] const Vector3& Min() const
    {
        return m_min;
    }

    [[nodiscard]] const Vector3& Max() const
    {
        return m_max;
    }

    [[nodiscard]] EAxis LongestAxis() const
    {
        const auto diff = m_max - m_min;

        const float xLength = diff.x;
        const float yLength = diff.y;
        const float zLength = diff.z;

        if ( xLength > yLength )
            return xLength > zLength ? EAxis::X : EAxis::Z;

        return yLength > zLength ? EAxis::Y : EAxis::Z;
    }

    [[nodiscard]] bool IsHit( const Ray& ray, Range<float> minMax ) const;

    AABB operator+( const Vector3& offset ) const
    {
        return { m_min + offset, m_max + offset };
    }

private:
    void PadToMinimums()
    {
        constexpr float delta    = .0001f;
        constexpr float toExpand = delta * .5f;

        if ( m_max.x - m_min.x < delta )
        {
            m_min.x -= toExpand;
            m_max.x += toExpand;
        }
        if ( m_max.y - m_min.y < delta )
        {
            m_min.y -= toExpand;
            m_max.y += toExpand;
        }
        if ( m_max.z - m_min.z < delta )
        {
            m_min.z -= toExpand;
            m_max.z += toExpand;
        }
    }
};
}  // namespace tsleddens