#include "Camera.h"

#include "HitResult.h"
#include "IMaterial.h"
#include "IRayTraceable.h"
#include "Ray.h"
#include "Win32Rasterizer.h"

using namespace tsleddens;

Camera::Camera(UINT imageWidth, UINT imageHeight)
{
    Resize(imageWidth, imageHeight);
}

void Camera::Resize(UINT imageWidth, UINT imageHeight)
{
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_aspectRatio = static_cast<float>(imageWidth) / static_cast<float>(imageHeight);

    const Point3 screenCenter = m_position + (m_fieldOfView * m_viewDirection);
    m_p0 = screenCenter + Point3(-m_aspectRatio, -1.f, 0.f);
    const Point3 p1 = screenCenter + Point3(m_aspectRatio, -1.f, 0.f);
    const Point3 p2 = screenCenter + Point3(-m_aspectRatio, 1.f, 0.f);

    m_uDelta = 1.f / static_cast<float>(imageWidth) * (p1 - m_p0);
    m_vDelta = 1.f / static_cast<float>(imageHeight) * (p2 - m_p0);
    m_frameCount = 1;
}

void Camera::TraceAndPlot(const IRayTraceable& world, const Win32Rasterizer& rasterizer)
{
    float reciprocalFrameCount = 1.f / static_cast<float>(m_frameCount + 1);
    for (UINT y = 0; y < m_imageHeight; ++y)
    {
        for (UINT x = 0; x < m_imageWidth; ++x)
        {
            Ray ray = GetRay(x, y);
            Color color = SampleColor(ray, world);
            rasterizer.PlotPixel(x, y, color, reciprocalFrameCount);
        }
    }
    m_frameCount++;
}


Ray Camera::GetRay(const UINT x, const UINT y) const
{
    const Vector3 randomOffset = GenerateRandomOffset();
    const Point3 pixelSample = m_p0
        + ((static_cast<float>(x) + randomOffset.x) * m_uDelta)
        + ((static_cast<float>(y) + randomOffset.y) * m_vDelta);
    const Vector3 direction = pixelSample - m_position;
    return Ray(m_position, direction);
}

Color Camera::SampleColor(const Ray& ray, const IRayTraceable& world) const
{
    HitResult hitResult;
    if (world.Intersect(ray, hitResult, 0.f, FLT_MAX))
    {
        if (m_isRenderNormalsEnabled)
        {
            return GetNormalColor(hitResult.GetNormal());
        }

        return hitResult.GetMaterial()->GetColor();
    }

    return GetNoHitColor(ray);
}

Color Camera::GetNormalColor(const Vector3& normal)
{
    return (normal + 1.f) * 0.5f;
}

Color Camera::GetNoHitColor(const Ray& ray)
{
    const Vector3 unitDirection = ray.GetDirection();
    const float a = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - a) * Color(1.0f) + a * Color(0.5f, 0.7f, 1.0f);
}
