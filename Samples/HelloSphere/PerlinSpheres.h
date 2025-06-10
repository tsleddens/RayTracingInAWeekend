#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Textures/ITexture.h"
#include "tsleddens/Textures/NoiseTexture.h"

using namespace tsleddens;

class PerlinSpheres final : public AppBase
{
    std::shared_ptr<ITexture> m_noiseTexture;
    std::unique_ptr<IMaterial> m_noiseMaterial;

public:

    PerlinSpheres(int width, int height, const wchar_t* title) :
        AppBase(width, height, title),
        m_noiseTexture(std::make_shared<NoiseTexture>(4.f)),
        m_noiseMaterial(std::make_unique<Lambertian>(m_noiseTexture))
    {
    }

    ~PerlinSpheres() override = default;

protected:

    void InitWorld(World& world) override
    {
        auto camera = GetCamera();
        camera.SetLookAt(Point3(0.f));
        camera.SetPosition(Point3(13.f, 2.f, 3.f));

        world.AddObject<Sphere>(Point3(0.f, -1000.f, 0.f), 1000.f, m_noiseMaterial.get());
        world.AddObject<Sphere>(Point3(0.f, 2.f, 0.f), 2.f, m_noiseMaterial.get());
    }
};
