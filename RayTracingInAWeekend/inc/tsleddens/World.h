#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
    struct IMaterial;

    class World: public IRayTraceable
    {
        std::shared_ptr<IRayTraceable> m_objects[1024];
        UINT m_count;
        AABB m_boundingBox{};

    public:
        World();

        template<typename TRayTraceable, typename... TArgs>
        void AddObject(TArgs&&... args);

        void AddObject(const std::shared_ptr<IRayTraceable>& object)
        {
            m_objects[m_count] = object;
            m_boundingBox = AABB(m_boundingBox, m_objects[m_count]->BoundingBox());
            m_count++;
        }

        void AddBox(const Point3& a, const Point3& b, IMaterial* pMaterial);

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override;

        [[nodiscard]] IMaterial* GetMaterial() const override;
        [[nodiscard]] bool HasFlippedNormals() const override { return false; }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }
        [[nodiscard]] UINT GetObjectCount() const { return m_count; }

        std::shared_ptr<IRayTraceable>* GetObjects(int& count) { count = m_count; return m_objects;  }
        std::shared_ptr<IRayTraceable>* GetObjects() { return m_objects;  }
    };
}

template <typename TRayTraceable, typename ... TArgs>
void tsleddens::World::AddObject(TArgs&&... args)
{
    static_assert(std::is_base_of_v<IRayTraceable, TRayTraceable>, "TRayTraceable must derive from IRayTraceable");
    m_objects[m_count] = std::make_shared<TRayTraceable>(std::forward<TArgs>(args)...);
    m_boundingBox = AABB(m_boundingBox, m_objects[m_count]->BoundingBox());
    m_count++;
}
