#pragma once

#define GLM_FORCE_INLINE
#define GLM_FORCE_SIMD_AVX2
#define GLM_FORCE_ALIGNED
#define GLM_ENABLE_EXPERIMENTAL

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>

using Vector3   = glm::vec3;
using Point3    = glm::vec3;
using Color     = glm::vec3;
using Pixel     = glm::vec2;
using ColorCode = uint32_t;

constexpr float EPSILON = 1e-4f;

enum EAxis : uint8_t
{
    X,
    Y,
    Z
};

inline float LinearToGamma( float linear )
{
    constexpr float gamma = 1.f / 2.2f;
    if ( linear > 0.f )
    {
        return std::pow( linear, gamma );  // Gamma correction
    }

    return 0.f;
}

inline ColorCode ColorToColorCode( const Color& color )
{
    const auto r = static_cast<ColorCode>( LinearToGamma( std::clamp( color.x, 0.f, 1.f ) ) * 255.999f ) & 0xff;
    const auto g = static_cast<ColorCode>( LinearToGamma( std::clamp( color.y, 0.f, 1.f ) ) * 255.999f ) & 0xff;
    const auto b = static_cast<ColorCode>( LinearToGamma( std::clamp( color.z, 0.f, 1.f ) ) * 255.999f ) & 0xff;
    return ( r << 16 ) + ( g << 8 ) + b;
}

inline float RandomFloat()
{
    return std::rand() / ( static_cast<float>( RAND_MAX ) + 1.f );
}

inline float RandomFloat( float min, float max )
{
    return min + ( max - min ) * RandomFloat();
}

inline int RandomInt( int min, int max )
{
    return static_cast<int>( RandomFloat( min, max + 1 ) );
}

inline Vector3 RandomVector3( float min = 0.f, float max = 1.f )
{
    return Vector3( RandomFloat( min, max ), RandomFloat( min, max ), RandomFloat( min, max ) );
}

inline bool IsNearZero( const Vector3& v, float epsilon = EPSILON )
{
    return std::abs( v.x ) < epsilon && std::abs( v.y ) < epsilon && std::abs( v.z ) < epsilon;
}

inline Vector3 RandomUnitVector3()
{
    while ( true )
    {
        Vector3 p             = RandomVector3( -1.f, 1.f );
        float   lengthSquared = glm::length2( p );
        if ( lengthSquared >= 1.f || lengthSquared < 0.00001f )
            continue;  // Skip points that are outside the unit sphere or too close to the origin.

        return glm::normalize( p );
    }
}

inline Vector3 RandomBounce( const Vector3& normal )
{
    Vector3 randomDirection = RandomUnitVector3();
    if ( glm::dot( randomDirection, normal ) < 0.f )
    {
        randomDirection = -randomDirection;  // Ensure the random direction is in the hemisphere defined by the normal.
    }
    return randomDirection;
}

inline Vector3 RandomCosineDirection()
{
    const float r1 = RandomFloat();
    const float r2 = RandomFloat();

    const float phi = 2 * glm::pi<float>() * r1;
    const float x   = std::cos( phi ) * std::sqrt( r2 );
    const float y   = std::sin( phi ) * std::sqrt( r2 );
    const float z   = std::sqrt( 1 - r2 );

    return Vector3( x, y, z );
}