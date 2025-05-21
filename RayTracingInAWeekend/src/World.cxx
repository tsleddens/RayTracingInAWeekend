#include "World.h"

#include "HitResult.h"
#include "Ray.h"

using namespace tsleddens;

World::World() :
    m_objects(),
    m_count(0)
{

}

bool World::Intersect(const Ray& ray, HitResult& hitResult, float minDistance, float maxDistance) const
{
    bool isHit = false;
    for (UINT i = 0; i < m_count; ++i)
    {
        if (m_objects[i]->Intersect(ray, hitResult, minDistance, maxDistance))
        {
            maxDistance = hitResult.GetDistance();
            isHit = true;
        }
    }

    return isHit;
}

IMaterial* World::GetMaterial() const
{
    return nullptr;
}
