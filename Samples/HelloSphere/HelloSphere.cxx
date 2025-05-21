#include "HelloSphere.h"

#include "tsleddens/HitResult.h"
#include "tsleddens/Material.h"
#include "tsleddens/Ray.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"

using namespace tsleddens;

void HelloSphere::OnUpdate()
{
    UpdateFps();
}

void HelloSphere::OnBeforeRender()
{
    const UINT imageWidth = GetWidth();
    const UINT imageHeight = GetHeight();
    const float aspectRatio = GetAspectRatio();

    constexpr float fieldOfView = 1.0f;

    constexpr Point3 cameraPositionE = Point3(0.f);
    constexpr Vector3 viewDirectionV = Vector3(0.f, 0.f, -1.f);
    constexpr Point3 screenCenterC = cameraPositionE + (fieldOfView * viewDirectionV);

    const Point3 p0 = screenCenterC + Point3(-aspectRatio, -1.f, 0.f);
    const Point3 p1 = screenCenterC + Point3(aspectRatio, -1.f, 0.f);
    const Point3 p2 = screenCenterC + Point3(-aspectRatio, 1.f, 0.f);

    const float uDelta = 1.0f / (static_cast<float>(imageWidth));
    const float vDelta = 1.0f / static_cast<float>(imageHeight);

    const std::unique_ptr<Material> redAlbedo = std::make_unique<Material>(Color(1.f, 0.f, 0.f));

    World world;

    world.AddObject<Sphere>(Point3(0.f, 0, -1.f), 0.5f, redAlbedo.get());
    world.AddObject<Sphere>(Point3(0.f, -100.5f, -1.f), 100.f, redAlbedo.get());

    for (UINT y = 0; y < imageHeight; ++y)
    {
        for (UINT x = 0; x < imageWidth; ++x)
        {
            const float u = static_cast<float>(x) * uDelta;
            const float v = static_cast<float>(y) * vDelta;

            const Point3 pixelCenter = p0 + (u * (p1 - p0)) + (v * (p2 - p0));
            const Vector3 direction = pixelCenter - cameraPositionE;

            const Ray ray = Ray(cameraPositionE, direction);
            HitResult hitResult;
            if (world.Intersect(ray, hitResult, 0.f , FLT_MAX))
            {
                // PlotPixel(x, y, hitResult.GetMaterial()->GetColor());
                auto normal = hitResult.GetNormal();
                auto normalColor = (normal + 1.f) * 0.5f;
                PlotPixel(x, y, ColorToColorCode(normalColor));
            }
            else
            {
                const Vector3 unitDirection = ray.GetDirection();
                const float a = 0.5f * (unitDirection.y + 1.0f);
                const Color color = (1.0f - a) * Color(1.0f) + a * Color(0.5f, 0.7f, 1.0f);
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
