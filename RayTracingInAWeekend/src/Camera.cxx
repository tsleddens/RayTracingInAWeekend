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

    Point3 screenCenter = m_position + (m_fieldOfView * m_viewDirection);
    m_p0 = screenCenter + Point3(-m_aspectRatio, -1.f, 0.f);
    m_p1 = screenCenter + Point3(m_aspectRatio, -1.f, 0.f);
    m_p2 = screenCenter + Point3(-m_aspectRatio, 1.f, 0.f);

    m_uDelta = 1.f / static_cast<float>(imageWidth);
    m_vDelta = 1.f / static_cast<float>(imageHeight);
}

void Camera::TraceAndPlot(const IRayTraceable& world, const Win32Rasterizer& rasterizer) const
{
    for (UINT y = 0; y < m_imageHeight; ++y)
    {
        for (UINT x = 0; x < m_imageWidth; ++x)
        {
            const float u = static_cast<float>(x) * m_uDelta;
            const float v = static_cast<float>(y) * m_vDelta;

            const Point3 pixelCenter = m_p0 + (u * (m_p1 - m_p0)) + (v * (m_p2 - m_p0));
            const Vector3 direction = pixelCenter - m_position;

            const Ray ray = Ray(m_position, direction);
            HitResult hitResult;
            if (world.Intersect(ray, hitResult, 0.f, FLT_MAX))
            {
                if (IsRenderNormalsEnabled())
                {
                    auto normal = hitResult.GetNormal();
                    auto normalColor = (normal + 1.f) * 0.5f;
                    rasterizer.PlotPixel(x, y, ColorToColorCode(normalColor));
                }
                else
                {
                    rasterizer.PlotPixel(x, y, hitResult.GetMaterial()->GetColor());
                }
            }
            else
            {
                const Vector3 unitDirection = ray.GetDirection();
                const float a = 0.5f * (unitDirection.y + 1.0f);
                const Color color = (1.0f - a) * Color(1.0f) + a * Color(0.5f, 0.7f, 1.0f);
                rasterizer.PlotPixel(x, y, color);
            }
        }
    }
}
