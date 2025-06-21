#pragma once

namespace tsleddens
{
template<typename T>
class Range
{
    T m_min;
    T m_max;

public:
    Range()
    : m_min( std::numeric_limits<T>::infinity() )
    , m_max( -std::numeric_limits<T>::infinity() )
    {}

    Range( T min, T max )
    : m_min( std::min( min, max ) )
    , m_max( std::max( min, max ) )
    {}

    Range( const Range<T>& a, const Range<T>& b )
    : m_min( a.m_min <= b.m_min ? a.m_min : b.m_min )
    , m_max( a.m_max >= b.m_max ? a.m_max : b.m_max )
    {}

    [[nodiscard]] const T& GetMin() const
    {
        return m_min;
    }
    [[nodiscard]] const T& GetMax() const
    {
        return m_max;
    }

    [[nodiscard]] T Length() const
    {
        return m_max - m_min;
    }

    void SetMin( const T& min )
    {
        m_min = min;
    }
    void SetMax( const T& max )
    {
        m_max = max;
    }

    Range<float> operator+( float displacement ) const
    {
        return Range( m_min + displacement, m_max + displacement );
    }

    static void Expand( Range<T>& toExpand, T delta )
    {
        float padding = delta * 0.5f;
        toExpand.m_min -= padding;
        toExpand.m_max += padding;
    }

    [[nodiscard]] bool IsInRange( const T& value, bool inclusive = true ) const
    {
        if ( inclusive )
        {
            return ( value >= m_min && value <= m_max );
        }

        return ( value > m_min && value < m_max );
    }
};
}  // namespace tsleddens
