#include "Camera.h"

#include "DeFocusDisk.h"
#include "HitResult.h"
#include "IRayTraceable.h"
#include "Materials/IMaterial.h"
#include "PDF.h"
#include "Ray.h"
#include "Win32Rasterizer.h"

using namespace tsleddens;

Camera::Camera( const UINT imageWidth, const UINT imageHeight )
: m_deFocusDisk( DeFocusDisk() )
{
    Resize( imageWidth, imageHeight );
}

void Camera::Resize( const UINT imageWidth, const UINT imageHeight )
{
    m_frameCount  = 1;
    m_imageWidth  = imageWidth;
    m_imageHeight = imageHeight;
    m_aspectRatio = static_cast<float>( imageWidth ) / static_cast<float>( imageHeight );

    const float theta          = glm::radians( m_verticalFov );
    const float h              = glm::tan( theta / 2.f );
    const float viewportHeight = 2.f * h * m_deFocusDisk.GetDistance();
    const float viewportWidth  = viewportHeight * m_aspectRatio;

    const Vector3 w = glm::normalize( m_position - m_lookAt );
    const Vector3 u = glm::normalize( glm::cross( m_upDirection, w ) );
    const Vector3 v = glm::cross( w, u );
    m_deFocusDisk.GenerateDisk( u, v );

    m_uDelta = viewportWidth * u;
    m_vDelta = viewportHeight * -v;

    m_p0 = m_position - ( m_deFocusDisk.GetDistance() * w ) - m_uDelta / 2.f - m_vDelta / 2.f;
}

void Camera::Render( const IRayTraceable& world, IRasterizer& rasterizer, IRayTraceable* lights )
{
    const float reciprocalFrameCount = 1.f / static_cast<float>( m_frameCount );
    for ( UINT y = 0; y < m_imageHeight; ++y )
    {
        for ( UINT x = 0; x < m_imageWidth; ++x )
        {
            Ray   ray   = GetRay( x, y );
            Color color = SampleColor( ray, world, 0, lights );
            rasterizer.PlotPixel( x, y, color, reciprocalFrameCount );
        }
    }
    m_frameCount++;
}

Ray Camera::GetRay( const UINT x, const UINT y ) const
{
    const float u = ( static_cast<float>( x ) + Halton( m_frameCount, 2 ) ) / static_cast<float>( m_imageWidth );
    const float v = ( static_cast<float>( y ) + Halton( m_frameCount, 3 ) ) / static_cast<float>( m_imageHeight );

    const Point3 focusPoint = m_p0 + u * m_uDelta + v * m_vDelta;

    const Point3  rayOrigin = m_deFocusDisk.GetPosition( m_position );
    const Vector3 direction = focusPoint - rayOrigin;

    return Ray( rayOrigin, direction );
}

Color Camera::SampleColor( const Ray& ray, const IRayTraceable& world, UINT currentBounces,
                           IRayTraceable* lights ) const
{
    if ( currentBounces == m_maxBounces )
    {
        return Color( 0.f );
    }

    Range<float> range( 0.001f, FLT_MAX );
    if ( HitResult hitResult; world.Intersect( ray, hitResult, range ) )
    {
        if ( m_isRenderNormalsEnabled )
        {
            return GetNormalColor( hitResult.GetNormal() );
        }

        ScatterResult scatterResult;
        Color         emissionColor =
            hitResult.GetMaterial()->Emitted( ray, hitResult, hitResult.u, hitResult.v, hitResult.GetIntersection() );

        if ( !hitResult.GetMaterial()->Scatter( ray, hitResult, scatterResult, m_frameCount) )
        {
            return emissionColor;
        }

        if ( scatterResult.SkipPdf )
        {
            return scatterResult.Attenuation * SampleColor( scatterResult.SkipPdfRay, world, ++currentBounces, lights );
        }

        auto       pLight = std::make_shared<HittablePDF>( lights, hitResult.GetIntersection() );
        MixturePDF mixturePdf( pLight, scatterResult.pPdf );

        Ray   scattered = Ray( hitResult.GetIntersection(), mixturePdf.Generate() );
        float pdfValue  = mixturePdf.Value( scattered.GetDirection() );

        if ( pdfValue < 1e-5f )
        {
            return emissionColor;
        }

        float scatteringPdf = hitResult.GetMaterial()->ScatteringPdf( ray, hitResult, scattered );

        Color sampleColor = SampleColor( scattered, world, ++currentBounces, lights );
        Color attenuation = scatterResult.Attenuation;

        if ( currentBounces > 5 )
        {
            float p = glm::clamp( glm::compMax( attenuation ), 0.1f, 1.f );
            if ( RandomFloat() > p )
            {
                return emissionColor;
            }
            attenuation /= p;
        }

        Color scatterColor = ( attenuation * scatteringPdf * sampleColor ) / pdfValue;

        return scatterColor + emissionColor;
    }

    return m_background;
}

Color Camera::GetNormalColor( const Vector3& normal )
{
    return ( normal + 1.f ) * 0.5f;
}

Color Camera::GetNoHitColor( const Ray& ray )
{
    const Vector3 unitDirection = ray.GetDirection();
    const float   a             = 0.5f * ( unitDirection.y + 1.0f );
    return ( 1.0f - a ) * Color( 1.0f ) + a * Color( 0.5f, 0.7f, 1.0f );
}