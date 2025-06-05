#include "World.h"

#include "HitResult.h"

using namespace tsleddens;

World::World() :
    m_objects(),
    m_count(0)
{

}

bool World::Intersect(const Ray& ray, HitResult& hitResult, Range<float>& range) const
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
