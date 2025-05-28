#include "HelloSphere.h"

#include "tsleddens/HitResult.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Lambertian.h"

using namespace tsleddens;

HelloSphere::HelloSphere(int width, int height, const wchar_t* title):
    Win32Rasterizer(width, height, title),
    m_camera(Camera(width, height)),
    m_world(World()),
    m_lambertian1(std::make_unique<Lambertian>(Color(0.5f, 0.5f, 0.5f))),
    m_lambertian2(std::make_unique<Lambertian>(Color(0.f, 1.f, 0.f)))
{
    // m_camera.EnableRenderNormals();
    m_world.AddObject<Sphere>(Point3(0.f, 0, -1.f), 0.5f, m_lambertian1.get());
    m_world.AddObject<Sphere>(Point3(0.f, -100.5f, -1.f), 100.f, m_lambertian2.get());
}

void HelloSphere::OnResize(UINT newWidth, UINT newHeight)
{
    m_camera.Resize(newWidth, newHeight);
}

void HelloSphere::OnUpdate()
{
    UpdateFps();
}

void HelloSphere::OnBeforeRender()
{
    m_camera.TraceAndPlot(m_world, *this);
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
