#include "World.h"

#include "HitResult.h"
#include "Quad.h"

using namespace tsleddens;

World::World() :
    m_objects(),
    m_count(0)
{

}

void World::AddBox(const Point3& a, const Point3& b, IMaterial* pMaterial)
{
    const Point3 min(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
    const Point3 max(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

    const Vector3 dx(max.x - min.x, 0.f, 0.f);
    const Vector3 dy(0.f, max.y - min.y, 0.f);
    const Vector3 dz(0.f, 0.f, max.z - min.z);

    AddObject<Quad>(Point3(min.x, min.y, max.z), dx, dy, pMaterial);
    AddObject<Quad>(Point3(max.x, min.y, max.z), -dz, dy, pMaterial);
    AddObject<Quad>(Point3(max.x, min.y, min.z), -dx, dy, pMaterial);
    AddObject<Quad>(Point3(min.x, min.y, min.z), dz, dy, pMaterial);
    AddObject<Quad>(Point3(min.x, max.y, max.z), dx, -dz, pMaterial);
    AddObject<Quad>(Point3(min.x, min.y, min.z), dx, dz, pMaterial);
}

bool World::Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const
{
    bool isHit = false;
    for (UINT i = 0; i < m_count; ++i)
    {
        if (m_objects[i]->Intersect(ray, hitResult, range))
        {
            isHit = true;
            range.SetMax(hitResult.GetDistance());
        }
    }

    return isHit;
}

IMaterial* World::GetMaterial() const
{
    return nullptr;
}
