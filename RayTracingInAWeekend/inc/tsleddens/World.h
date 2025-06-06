#pragma once

#include "IRayTraceable.h"

namespace tsleddens
{
    class World: public IRayTraceable
    {
        std::unique_ptr<IRayTraceable> m_objects[1024];
        UINT m_count;
        AABB m_boundingBox{};

    public:
        World();

        template<typename Factory>
        void AddObject(Factory&& factory);

        template<typename TRayTraceable, typename... TArgs>
        void AddObject(TArgs&&... args);

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float>& range) const override;

        [[nodiscard]] IMaterial* GetMaterial() const override;
        [[nodiscard]] bool HasFlippedNormals() const override { return false; }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }
    };
}

template <typename Factory>
void tsleddens::World::AddObject(Factory&& factory)
{
    m_objects[m_count] = factory();
    m_boundingBox = AABB(m_boundingBox, m_objects[m_count]->BoundingBox());
    m_count++;
}

template <typename TRayTraceable, typename ... TArgs>
void tsleddens::World::AddObject(TArgs&&... args)
{
    static_assert(std::is_base_of_v<IRayTraceable, TRayTraceable>, "TRayTraceable must derive from IRayTraceable");
    m_objects[m_count] = std::make_unique<TRayTraceable>(std::forward<TArgs>(args)...);
    m_boundingBox = AABB(m_boundingBox, m_objects[m_count]->BoundingBox());
    m_count++;
}
