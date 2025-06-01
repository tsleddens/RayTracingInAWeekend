#include "HelloSphere.h"

#include "tsleddens/HitResult.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Dielectric.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Materials/Metal.h"

using namespace tsleddens;

HelloSphere::HelloSphere(int width, int height, const wchar_t* title):
    Win32Rasterizer(width, height, title),
    m_camera(Camera(width, height)),
    m_world(World()),
    m_materialGround(std::make_unique<Lambertian>(Color(0.8f, 0.8f, 0.0f))),
    m_materialCenter(std::make_unique<Lambertian>(Color(0.1f, 0.2f, 0.5f))),
    m_materialLeft(std::make_unique<Dielectric>(1.5f)),
    m_materialBubble(std::make_unique<Dielectric>(1.f / 1.5f)),
    m_materialRight(std::make_unique<Metal>(Color(0.8f, 0.6f, 0.2f), 1.0f))
{
    // m_camera.EnableRenderNormals();
    m_world.AddObject<Sphere>(Point3( 0.f, -100.5f, -1.0f), 100.0f, m_materialGround.get());
    m_world.AddObject<Sphere>(Point3( 0.f,    0.0f, -1.2f),   0.5f, m_materialCenter.get());
    m_world.AddObject<Sphere>(Point3(-1.f,    0.0f, -1.0f),   0.5f, m_materialLeft.get());
    m_world.AddObject<Sphere>(Point3(-1.f,    0.0f, -1.0f),   0.4f, m_materialBubble.get());
    m_world.AddObject<Sphere>(Point3( 1.f,    0.0f, -1.0f),   0.5f, m_materialRight.get());
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
