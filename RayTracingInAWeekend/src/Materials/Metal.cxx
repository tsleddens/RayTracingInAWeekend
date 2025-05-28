#include "Materials/Metal.h"

#include "HitResult.h"

using namespace tsleddens;

bool Metal::Scatter(const Ray& ray, const HitResult& hitResult, Color& attenuation, Ray& scattered) const
{
    Vector3 reflected = glm::reflect(glm::normalize(ray.GetDirection()), hitResult.GetNormal());
    scattered = Ray(hitResult.GetIntersection(), reflected);
    attenuation = m_albedo;
    return true;
}
