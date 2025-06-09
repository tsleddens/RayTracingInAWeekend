#include "HelloSphere.h"

#include "tsleddens/HitResult.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Dielectric.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Materials/Metal.h"
#include "tsleddens/Textures/CheckerTexture.h"

using namespace tsleddens;

HelloSphere::HelloSphere(int width, int height, const wchar_t* title):
    AppBase(width, height, title),
    m_checkerTexture(std::make_shared<CheckerTexture>(0.32f, Color(.2f, .3f, .1f), Color(.9f))),
    m_materialGround(std::make_unique<Lambertian>(m_checkerTexture)),
    m_material1(std::make_unique<Dielectric>(1.5f)),
    m_material2(std::make_unique<Lambertian>(Color(0.4f, 0.2f, 0.1f))),
    m_material3(std::make_unique<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f))
{
    GetCamera().SetDeFocusValues(10.0f, 0.6f);
}

void HelloSphere::InitWorld(tsleddens::World& world)
{
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
                world.AddObject<Sphere>(center, 0.2f, m_materials[index].get());
            }
        }
    }

    world.AddObject<Sphere>(Point3(0.f, -1000.f, 0.f), 1000.f, m_materialGround.get());
    world.AddObject<Sphere>(Point3(0.f, 1.f, 0.f), 1.f, m_material1.get());
    world.AddObject<Sphere>(Point3(-4.f, 1.f, 0.f), 1.f, m_material2.get());
    world.AddObject<Sphere>(Point3(4.f, 1.f, 0.f), 1.f, m_material3.get());
}