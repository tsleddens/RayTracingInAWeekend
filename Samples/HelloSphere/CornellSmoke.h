#pragma once
#include "tsleddens/AppBase.h"
#include "tsleddens/ConstantMedium.h"
#include "tsleddens/Quad.h"
#include "tsleddens/RotateY.h"
#include "tsleddens/Translate.h"
#include "tsleddens/Materials/DiffuseLight.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Textures/ImageTexture.h"

using namespace tsleddens;

class CornellSmoke final : public AppBase
{
    std::unique_ptr<IMaterial> m_red;
    std::unique_ptr<IMaterial> m_white;
    std::unique_ptr<IMaterial> m_green;
    std::unique_ptr<IMaterial> m_light;

public:

    CornellSmoke(int width, int height, const wchar_t* title) :
        AppBase(width, height, title),
        m_red(std::make_unique<Lambertian>(.65f, .05f, .05f)),
        m_white(std::make_unique<Lambertian>(.73f)),
        m_green(std::make_unique<Lambertian>(.12f, .45f, .15f)),
        m_light(std::make_unique<DiffuseLight>(7.f))
    {
    }

    ~CornellSmoke() override = default;

protected:

    void InitWorld(World& world) override
    {
        auto camera = GetCamera();
        camera->SetPosition(278.f, 278.f, -800.f);
        camera->SetLookAt(278.f, 278.f, 0.f);
        camera->SetVerticalFieldOfView(40.f);

        world.AddObject<Quad>(Point3(555.f, 0.f, 0.f), Vector3(0.f, 555.f, 0.f), Vector3(0.f, 0.f, 555.f), m_green.get());
        world.AddObject<Quad>(Point3(0.f), Vector3(0.f, 555.f, 0.f), Vector3(0.f, 0.f, 555.f), m_red.get());
        world.AddObject<Quad>(Point3(113.f, 554.f, 127.f), Vector3(330.f, 0.f, 0.f), Vector3(0.f, 0.f, 305.f), m_light.get());
        world.AddObject<Quad>(Point3(0.f), Vector3(555.f, 0.f, 0.f), Vector3(0.f, 0.f, 555.f), m_white.get());
        world.AddObject<Quad>(Point3(555.f), Vector3(-555.f, 0.f, 0.f), Vector3(0.f, 0.f, -555.f), m_white.get());
        world.AddObject<Quad>(Point3(0.f, 0.f, 555.f), Vector3(555.f, 0.f, 0.f), Vector3(0.f, 555.f, 0.f), m_white.get());

        std::shared_ptr<IRayTraceable> box1 = std::make_shared<Box>(Point3(0.f), Point3(165.f, 330.f, 165.f), m_white.get());
        box1 = std::make_shared<RotateY>(box1, 15.f);
        box1 = std::make_shared<Translate>(box1, Point3(265.f, 0.f, 295.f));
        world.AddObject(std::make_shared<ConstantMedium>(box1, 0.01f, Color(0.f)));

        std::shared_ptr<IRayTraceable> box2 = std::make_shared<Box>(Point3(0.f), Point3(165.f), m_white.get());
        box2 = std::make_shared<RotateY>(box2, -18.f);
        box2 = std::make_shared<Translate>(box2, Point3(130.f, 0.f, 65.f));
        world.AddObject(std::make_shared<ConstantMedium>(box2, 0.01f, Color(1.f)));
    }
};
