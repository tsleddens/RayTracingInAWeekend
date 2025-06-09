#include "BouncingSpheres.h"

#include "tsleddens/HitResult.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Textures/CheckerTexture.h"

using namespace tsleddens;

BouncingSpheres::BouncingSpheres(int width, int height, const wchar_t* title) :
    Win32Rasterizer(width, height, title),
    m_camera(Camera(width, height)),
    m_world(World()),
    m_checkerTexture(std::make_shared<CheckerTexture>(0.32f, Color(.2f, .3f, .1f), Color(.9f))),
    m_materialGround(std::make_unique<Lambertian>(m_checkerTexture))
{
    m_world.AddObject<Sphere>(Point3(0.f, -10.f, 0.f), 10.f, m_materialGround.get());
    m_world.AddObject<Sphere>(Point3(0.f, 10.f, 0.f), 10.f, m_materialGround.get());
    m_camera.SetLookAt(Point3(0.f));
    m_camera.SetPosition(Point3(13.f, 2.f, 3.f));

    m_bvh = BvhNode(m_world, 0, m_world.GetObjectCount());
}

void BouncingSpheres::OnResize(UINT newWidth, UINT newHeight)
{
    m_camera.Resize(newWidth, newHeight);
    m_frameCount = 0;
}

void BouncingSpheres::OnBeforeRender()
{
    m_camera.TraceAndPlot(m_bvh, *this);
}

void BouncingSpheres::OnAfterRender()
{
    UpdateFps();
}

void BouncingSpheres::UpdateFps()
{
    ++m_frameCount;
    static ULONGLONG framesPassed = m_frameCount;
    static ULONGLONG prevCount = GetTickCount();
    if (GetTickCount64() - prevCount > 1000) // only update every second
    {
        static wchar_t buffer[256] = {};
        (void)swprintf_s(buffer, L"FPS: %d, Samples: %llu", static_cast<int>(m_frameCount - framesPassed), m_frameCount);
        SetWindowTitle(buffer);
        framesPassed = m_frameCount;
        prevCount = GetTickCount64();
    }
}
