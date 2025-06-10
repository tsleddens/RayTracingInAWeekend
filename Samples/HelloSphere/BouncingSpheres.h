#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/Sphere.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Textures/CheckerTexture.h"
#include "tsleddens/Textures/ITexture.h"

class BouncingSpheres final : public tsleddens::AppBase
{
    std::shared_ptr<tsleddens::ITexture> m_checkerTexture;
    std::unique_ptr<tsleddens::IMaterial> m_materialGround;

public:

    BouncingSpheres(int width, int height, const wchar_t* title) :
        AppBase(width, height, title),
        m_checkerTexture(std::make_shared<tsleddens::CheckerTexture>(0.32f, Color(.2f, .3f, .1f), Color(.9f))),
        m_materialGround(std::make_unique<tsleddens::Lambertian>(m_checkerTexture))
    {
    }

    ~BouncingSpheres() override = default;

protected:

    void InitWorld(tsleddens::World& world) override
    {
        world.AddObject<tsleddens::Sphere>(Point3(0.f, -10.f, 0.f), 10.f, m_materialGround.get());
        world.AddObject<tsleddens::Sphere>(Point3(0.f, 10.f, 0.f), 10.f, m_materialGround.get());

        GetCamera()->SetBackgroundColor(.7f, .8f, 1.f);
    }
};
