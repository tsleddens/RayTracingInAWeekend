#include "HelloSphere.h"
#include "tsleddens/Ray.h"
#include "tsleddens/Sphere.h"

using namespace tsleddens;

void HelloSphere::OnUpdate()
{
    UpdateFps();
}

void HelloSphere::OnBeforeRender()
{
    const UINT width = GetWidth();
    const UINT height = GetHeight();
    const UINT pixelCount = GetPixelCount();

    constexpr float focalLength = 1.0f;
    constexpr float viewportHeight = 2.0f;
    const float viewportWidth = GetAspectRatio() * viewportHeight;

    constexpr auto origin = Point3(0, 0, 0);
    constexpr auto vertical = Vector3(0, viewportHeight, 0);
    const auto horizontal = Vector3(viewportWidth, 0, 0);
    const auto lowerLeftCorner = origin - horizontal / 2.0f - vertical / 2.0f - Vector3(0, 0, focalLength);
    const Sphere sphere(Point3(0, 0, -1.0f), 0.5f, Color(1, 0, 0));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const float u = static_cast<float>(x) / (static_cast<float>(width) - 1.0f);
            const float v = static_cast<float>(y) / (static_cast<float>(height) - 1.0f);
            const Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);

            const Vector3 unitDirection = r.NormalizedDirection();
            const float t = 0.5f * (unitDirection.y + 1.0f);

            ColorCode hitColor = sphere.Intersect(r);
            if(hitColor > 0)
            {
                PlotPixel(x, y, hitColor);
            }
            else
            {
                const Color color = (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
                PlotPixel(x, y, color);
            }
        }
    }
}

void HelloSphere::UpdateFps() const
{
    static ULONGLONG frameCount = 0;
    ++frameCount;
    static ULONGLONG framesPast = frameCount;
    static ULONGLONG prevCount = GetTickCount();
    if (GetTickCount64() - prevCount > 1000) // only update every second
    {
        static wchar_t buffer[256] = {};
        (void)swprintf_s(buffer, L"Raster Surface. FPS: %d", static_cast<int>(frameCount - framesPast));
        SetWindowTitle(buffer);
        framesPast = frameCount;
        prevCount = GetTickCount64();
    }
}