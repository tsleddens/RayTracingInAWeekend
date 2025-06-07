#pragma once
#include "IRayTraceable.h"

namespace tsleddens
{
    class World;

    class BvhNode : public IRayTraceable
    {
        std::shared_ptr<IRayTraceable> m_left = nullptr;
        std::shared_ptr<IRayTraceable> m_right = nullptr;
        AABB m_boundingBox{};

    public:
        BvhNode() = default;
        BvhNode(World& world, size_t start, size_t end);

        [[nodiscard]] bool Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const override;
        [[nodiscard]] IMaterial* GetMaterial() const override { return nullptr; }
        [[nodiscard]] bool HasFlippedNormals() const override { return false; }
        [[nodiscard]] const AABB& BoundingBox() const override { return m_boundingBox; }

        static [[nodiscard]] bool BoxCompare(const IRayTraceable* a, const IRayTraceable* b, EAxis axis)
        {
            return a->BoundingBox().AxisRange(axis).GetMin() < b->BoundingBox().AxisRange(axis).GetMin();
        }

        static [[nodiscard]] bool BoxXCompare(const IRayTraceable* a, const IRayTraceable* b)
        {
            return BoxCompare(a, b, EAxis::X);
        }

        static [[nodiscard]] bool BoxYCompare(const IRayTraceable* a, const IRayTraceable* b)
        {
            return BoxCompare(a, b, EAxis::Y);
        }

        static [[nodiscard]] bool BoxZCompare(const IRayTraceable* a, const IRayTraceable* b)
        {
            return BoxCompare(a, b, EAxis::Z);
        }
    };
}
