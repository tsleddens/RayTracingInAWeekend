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
    m_materialGround(std::make_unique<Lambertian>(Color(0.5f))),
    m_material1(std::make_unique<Dielectric>(1.5f)),
    m_material2(std::make_unique<Lambertian>(Color(0.4f, 0.2f, 0.1f))),
    m_material3(std::make_unique<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f))
{
    // m_camera.EnableRenderNormals();

    int offset = -11;

    for (int i = 0; i < 23; ++i)
    {
        for (int j = 0; j < 23; ++j)
        {
            float randomMaterial = RandomFloat();
            Point3 center(i + offset + 0.9f * RandomFloat(), 0.2f, j + offset + 0.9f * RandomFloat());

            int index = i * 23 + j;
            if (glm::length(center - Point3(4.f, 0.2f, 0.f)) > 0.9f)
            {
                if (randomMaterial < 0.8f) // diffuse
                {
                    Color albedo = RandomVector3() * RandomVector3();
                    m_materials[index] = std::make_unique<Lambertian>(albedo);
                }
                else if (randomMaterial < 0.95f) // metal
                {
                    Color albedo = RandomVector3(0.5f, 1.f);
                    float fuzz = RandomFloat(0.f, 0.5f);
                    m_materials[index] = std::make_unique<Metal>(albedo, fuzz);
                }
                else // glass
                {
                    m_materials[index] = std::make_unique<Dielectric>(1.5f);
                }
                m_world.AddObject<Sphere>(center, 0.2f, m_materials[index].get());
            }
        }
    }

    m_world.AddObject<Sphere>(Point3( 0.f, -1000.f, 0.f), 1000.f, m_materialGround.get());
    m_world.AddObject<Sphere>(Point3(0.f, 1.f, 0.f), 1.f, m_material1.get());
    m_world.AddObject<Sphere>(Point3(-4.f, 1.f, 0.f), 1.f, m_material2.get());
    m_world.AddObject<Sphere>(Point3( 4.f, 1.f, 0.f), 1.f, m_material3.get());
    m_camera.SetDeFocusValues(10.0f, 0.6f);

    m_bvh = BvhNode(m_world, 0, m_world.GetObjectCount());
}

void HelloSphere::OnResize(UINT newWidth, UINT newHeight)
{
    m_camera.Resize(newWidth, newHeight);
    m_frameCount = 0;
}

void HelloSphere::OnBeforeRender()
{
    m_camera.TraceAndPlot(m_bvh, *this);
}

void HelloSphere::OnAfterRender()
{
    UpdateFps();
}

void HelloSphere::UpdateFps()
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
