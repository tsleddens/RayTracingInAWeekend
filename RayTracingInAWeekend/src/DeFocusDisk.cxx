#include "DeFocusDisk.h"

using namespace tsleddens;

DeFocusDisk::DeFocusDisk( const float angle, const float distance )
: m_angle( angle )
, m_distance( distance )
, m_u()
, m_v()
{}

Point3 DeFocusDisk::GetPosition( const Point3& position ) const
{
    if ( m_angle <= 0.f )
    {
        return position;
    }

    const Vector3 p = RandomVector3InUnitDisk();
    return position + ( p.x * m_u ) + ( p.y * m_v );
}

void DeFocusDisk::GenerateDisk( const Vector3& u, const Vector3& v )
{
    const float deFocusRadius = m_distance * std::tan( glm::radians( m_angle / 2.f ) );

    m_u = u * deFocusRadius;
    m_v = v * deFocusRadius;
}

Vector3 DeFocusDisk::RandomVector3InUnitDisk()
{
    while ( true )
    {
        if ( auto p = Vector3( RandomFloat( -1.f, 1.f ), RandomFloat( -1.f, 1.f ), 0.f ); glm::length2( p ) < 1.f )
            return p;
    }
}
