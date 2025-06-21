#pragma once
#include "ColorTexture.h"
#include "ITexture.h"

namespace tsleddens
{
class CheckerTexture : public ITexture
{
    float                     m_invScale;
    std::shared_ptr<ITexture> m_even;
    std::shared_ptr<ITexture> m_odd;

public:
    CheckerTexture( float scale, const std::shared_ptr<ITexture>& even, const std::shared_ptr<ITexture>& odd )
    : m_invScale( 1.f / scale )
    , m_even( even )
    , m_odd( odd )
    {}

    CheckerTexture( float scale, const Color& even, const Color& odd )
    : m_invScale( 1.f / scale )
    , m_even( std::make_shared<ColorTexture>( even ) )
    , m_odd( std::make_shared<ColorTexture>( odd ) )
    {}

    Color Value( float u, float v, const Point3& p ) const override
    {
        int x = static_cast<int>( std::floor( m_invScale * p.x ) );
        int y = static_cast<int>( std::floor( m_invScale * p.y ) );
        int z = static_cast<int>( std::floor( m_invScale * p.z ) );

        bool isEven = ( x + y + z ) % 2 == 0;

        return isEven ? m_even->Value( u, v, p ) : m_odd->Value( u, v, p );
    }
};
}  // namespace tsleddens
