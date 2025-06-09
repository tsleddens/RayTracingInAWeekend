#include "BouncingSpheres.h"

#include "tsleddens/Sphere.h"
#include "tsleddens/Materials/Lambertian.h"
#include "tsleddens/Textures/CheckerTexture.h"

using namespace tsleddens;

BouncingSpheres::BouncingSpheres(int width, int height, const wchar_t* title) :
    AppBase(width, height, title),
    m_checkerTexture(std::make_shared<CheckerTexture>(0.32f, Color(.2f, .3f, .1f), Color(.9f))),
    m_materialGround(std::make_unique<Lambertian>(m_checkerTexture))
{
}

void BouncingSpheres::InitWorld(World& world)
{
    world.AddObject<Sphere>(Point3(0.f, -10.f, 0.f), 10.f, m_materialGround.get());
    world.AddObject<Sphere>(Point3(0.f, 10.f, 0.f), 10.f, m_materialGround.get());
}
