#pragma once

#include "tsleddens/AppBase.h"
#include "tsleddens/Quad.h"
#include "tsleddens/World.h"
#include "tsleddens/Materials/Lambertian.h"

using namespace tsleddens;

class Quads final : public AppBase
{
    std::unique_ptr<IMaterial> m_leftRed;
    std::unique_ptr<IMaterial> m_backGreen;
    std::unique_ptr<IMaterial> m_rightBlue;
    std::unique_ptr<IMaterial> m_upperOrange;
    std::unique_ptr<IMaterial> m_lowerTeal;

public:
    Quads(int width, int height, const wchar_t* title) :
        AppBase(width, height, title),
        m_leftRed(std::make_unique<Lambertian>(Color(1.f, .2f, .2f))),
        m_backGreen(std::make_unique<Lambertian>(Color(.2f, 1.f, .2f))),
        m_rightBlue(std::make_unique<Lambertian>(Color(.2f, .2f, 1.f))),
        m_upperOrange(std::make_unique<Lambertian>(Color(1.f, .5f, 0.f))),
        m_lowerTeal(std::make_unique<Lambertian>(Color(.2f, .8f, .8f)))
    {
    }

    ~Quads() override = default;

protected:
    void InitWorld(World& world) override
    {
        auto camera = GetCamera();
        camera->SetLookAt(Point3(0.f));
        camera->SetPosition(Point3(0.f, 0.f, 9.f));
        camera->SetVerticalFieldOfView(80.f);

        world.AddObject<Quad>(Point3(-3.f, -2.f, 5.f), Vector3(0.f, 0.f, -4.f), Vector3(0.f, 4.f,  0.f), m_leftRed.get());
        world.AddObject<Quad>(Point3(-2.f, -2.f, 0.f), Vector3(4.f, 0.f,  0.f), Vector3(0.f, 4.f,  0.f), m_backGreen.get());
        world.AddObject<Quad>(Point3( 3.f, -2.f, 1.f), Vector3(0.f, 0.f,  4.f), Vector3(0.f, 4.f,  0.f), m_rightBlue.get());
        world.AddObject<Quad>(Point3(-2.f,  3.f, 1.f), Vector3(4.f, 0.f,  0.f), Vector3(0.f, 0.f,  4.f), m_upperOrange.get());
        world.AddObject<Quad>(Point3(-2.f, -3.f, 5.f), Vector3(4.f, 0.f,  0.f), Vector3(0.f, 0.f, -4.f), m_lowerTeal.get());
    }
};
