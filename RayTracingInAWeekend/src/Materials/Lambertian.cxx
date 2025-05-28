#include "Materials/Lambertian.h"

#include "HitResult.h"

using namespace tsleddens;

bool Lambertian::Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const
{
    Vector3 scatterDirection = hitResult.GetNormal() + RandomUnitVector3();

    scattered = Ray(hitResult.GetIntersection(), scatterDirection);
    attenuation = m_albedo;
    return true;
}
