#pragma once
#include "ONB.h"

namespace tsleddens
{
    struct PDF
    {
        virtual ~PDF() = default;

        [[nodiscard]] virtual float Value(const Vector3& direction) const = 0;
        [[nodiscard]] virtual Vector3 Generate() const = 0;
    };

    class HittablePDF : public PDF
    {
        Point3 m_origin;
        IRayTraceable* m_objects;

    public:
        HittablePDF(IRayTraceable* objects, const Point3& origin) :
            m_origin(origin),
            m_objects(objects){}

        [[nodiscard]] float Value(const Vector3& direction) const override
        {
            return m_objects->PdfValue(m_origin, direction);
        }

        [[nodiscard]] Vector3 Generate() const override
        {
            return m_objects->Random(m_origin);
        }

    };

    class SpherePDF : public PDF
    {
    public:
        [[nodiscard]] float Value(const Vector3&) const override
        {
            return 1.f / (4.f * glm::pi<float>());
        }

        [[nodiscard]] Vector3 Generate() const override
        {
            return RandomUnitVector3();
        }
    };

    class CosinePDF : public PDF
    {
        ONB m_uvw;

    public:
        CosinePDF(const Vector3& w) :
            m_uvw(w)
        {
        }

        [[nodiscard]] float Value(const Vector3& direction) const override
        {
            float cosTheta = glm::dot(glm::normalize(direction), m_uvw.W());
            return std::fmax(0.f, cosTheta / glm::pi<float>());
        }

        [[nodiscard]] Vector3 Generate() const override
        {
            return m_uvw.Transform(RandomCosineDirection());
        }
    };
}
