#include "Camera.h"

#include "DeFocusDisk.h"
#include "HitResult.h"
#include "Materials/IMaterial.h"
#include "IRayTraceable.h"
#include "Ray.h"
#include "Win32Rasterizer.h"

using namespace tsleddens;

Camera::Camera(UINT imageWidth, UINT imageHeight) :
    m_deFocusDisk(DeFocusDisk())
{
    Resize(imageWidth, imageHeight);
}

void Camera::Resize(UINT imageWidth, UINT imageHeight)
{
    m_frameCount = 1;
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_aspectRatio = static_cast<float>(imageWidth) / static_cast<float>(imageHeight);

    const float theta = glm::radians(m_verticalFov);
    const float h = glm::tan(theta / 2.f);
    const float viewportHeight = 2.f * h * m_deFocusDisk.GetDistance();
    const float viewportWidth = viewportHeight * m_aspectRatio;

    const Vector3 w = glm::normalize(m_position - m_lookAt);
    const Vector3 u = glm::normalize(glm::cross(m_upDirection, w));
    const Vector3 v = glm::cross(w, u);
    m_deFocusDisk.GenerateDisk(u, v);

    const Vector3 horizontal = viewportWidth * u;
    const Vector3 vertical = viewportHeight * v;

    m_uDelta = horizontal / static_cast<float>(imageWidth);
    m_vDelta = vertical / static_cast<float>(imageHeight);

    m_p0 = m_position - (m_deFocusDisk.GetDistance() * w) - horizontal / 2.f - vertical / 2.f;
}

void Camera::TraceAndPlot(const IRayTraceable& world, Win32Rasterizer& rasterizer)
{
    float reciprocalFrameCount = 1.f / static_cast<float>(m_frameCount);
    for (UINT y = 0; y < m_imageHeight; ++y)
    {
        for (UINT x = 0; x < m_imageWidth; ++x)
        {
            Ray ray = GetRay(x, y);
            Color color = SampleColor(ray, world, 0);
            rasterizer.PlotPixel(x, y, color, reciprocalFrameCount);
        }
    }
    m_frameCount++;
}

Ray Camera::GetRay(const UINT x, const UINT y) const
{
    const Vector3 randomOffset = GenerateRandomOffset();
    const Point3 focusPoint = m_p0
        + ((static_cast<float>(x) + randomOffset.x) * m_uDelta)
        + ((static_cast<float>(y) + randomOffset.y) * m_vDelta);

    const Point3 rayOrigin = m_deFocusDisk.GetPosition(m_position);
    const Vector3 direction = focusPoint - rayOrigin;

    return Ray(rayOrigin, direction);
}

Color Camera::SampleColor(const Ray& ray, const IRayTraceable& world, UINT currentBounces) const
{
    if (currentBounces == m_maxBounces)
    {
        return Color(0.f);
    }

    Range<float> range(0.001f, FLT_MAX);
    HitResult hitResult;
    if (world.Intersect(ray, hitResult, range))
    {
        if (m_isRenderNormalsEnabled)
        {
            return GetNormalColor(hitResult.GetNormal());
        }

        Ray scattered;
        Color attenuation;
        Color emissionColor = hitResult.GetMaterial()->Emitted(hitResult.u, hitResult.v, hitResult.GetIntersection());

        if (!hitResult.GetMaterial()->Scatter(ray, hitResult, attenuation, scattered))
        {
            return emissionColor;
        }

        Color scatterColor = attenuation * SampleColor(scattered, world, ++currentBounces);
        return scatterColor + emissionColor;
    }

    return m_background;
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
