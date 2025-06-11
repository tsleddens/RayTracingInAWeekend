#include "Quad.h"

#include "HitResult.h"
#include "Ray.h"

using namespace tsleddens;

bool Quad::Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const
{
    const float denominator = glm::dot(m_normal, ray.GetDirection());

    if (std::fabs(denominator) >= 1e-8f)
    {
        float t = (m_d - glm::dot(m_normal, ray.GetOrigin())) / denominator;
        if (range.IsInRange(t, false))
        {
            const Point3 intersection = ray.At(t);
            const Vector3 planarHitpoint = intersection - m_q;
            const float alpha = glm::dot(m_w, glm::cross(planarHitpoint, m_v));

            if (float beta = glm::dot(m_w, glm::cross(m_u, planarHitpoint)); IsInterior(alpha, beta, hitResult))
            {
                hitResult.SetDistance(t, ray);
                hitResult.SetFaceNormal(ray, m_normal, false);
                hitResult.SetMaterial(this->GetMaterial());
                return true;
            }
        }
    }

    return false;
}

bool Quad::IsInterior(float a, float b, HitResult& hitResult)
{
    Range<float> unitRange(0.f, 1.f);

    const bool isInterior = unitRange.IsInRange(a) && unitRange.IsInRange(b);
    if (isInterior)
    {
        hitResult.u = a;
        hitResult.v = b;
    }

    return isInterior;
}

bool Box::Intersect(const Ray& ray, HitResult& hitResult, Range<float> range) const
{
    bool isHit = false;
    for (UINT i = 0; i < 6; ++i)
    {
        if (m_faces[i]->Intersect(ray, hitResult, range))
        {
            isHit = true;
            range.SetMax(hitResult.GetDistance());
        }
    }

    return isHit;
}
