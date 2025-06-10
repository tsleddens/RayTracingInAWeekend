#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Textures/ImageTexture.h"
#include "tsleddens/Textures/ITexture.h"

using namespace tsleddens;

class Earth final : public AppBase
{
    std::shared_ptr<ITexture> m_earthTexture;
    std::unique_ptr<IMaterial> m_earthMaterial;

public:

    Earth(int width, int height, const wchar_t* title) :
        AppBase(width, height, title),
        m_earthTexture(std::make_shared<ImageTexture>("images/earthmap.jpg")),
        m_earthMaterial(std::make_unique<Lambertian>(m_earthTexture))
    {
    }

    ~Earth() override = default;

protected:

    void InitWorld(World& world) override
    {
        auto camera = GetCamera();
        camera->SetLookAt(Point3(0.f));
        camera->SetPosition(Point3(0.f, 0.f, 12.f));

        world.AddObject<Sphere>(Point3(0.f), 2.f, m_earthMaterial.get());
    }
};
