#pragma once
#include "Defines.h"

namespace tsleddens
{
    class Win32Rasterizer;
    class Ray;
    struct IRayTraceable;

    class Camera
    {
        bool m_isRenderNormalsEnabled = false;

        UINT m_imageWidth = 0;
        UINT m_imageHeight = 0;
        UINT m_samplesPerPixel = 1;
        UINT m_frameCount = 1;

        float m_aspectRatio = 0.f;
        float m_fieldOfView = 1.f;

        Vector3 m_uDelta = Vector3(0);
        Vector3 m_vDelta = Vector3(0);
        Vector3 m_viewDirection = Vector3(0.f, 0.f, -1.f);

        Point3 m_position = Point3(0.f);
        Point3 m_p0 = Point3(0.f);

    public:
        Camera(UINT imageWidth, UINT imageHeight);

        void Resize(UINT imageWidth, UINT imageHeight);

        void SetFieldOfView(const float fov) { m_fieldOfView = fov; }
        void SetSamplesPerPixel(UINT value) { m_samplesPerPixel = value < 1 ? 1 : value; }

        void TraceAndPlot(const IRayTraceable& world, const Win32Rasterizer& rasterizer);
        void EnableRenderNormals() { m_isRenderNormalsEnabled = true; }
        void DisableRenderNormals() { m_isRenderNormalsEnabled = false; }

        UINT GetSamplesPerPixel() const { return m_samplesPerPixel; }

        bool IsRenderNormalsEnabled() const { return m_isRenderNormalsEnabled; }
    private:
        Ray GetRay(const UINT x, const UINT y) const;
        Color SampleColor(const Ray& ray, const IRayTraceable& world) const;
        static Color GetNormalColor(const Vector3& normal);
        static Color GetNoHitColor(const Ray& ray);
        static Vector3 GenerateRandomOffset(float minRange = 0.f, float maxRange = 1.f);
    };

    inline Vector3 Camera::GenerateRandomOffset(float minRange, float maxRange)
    {
        return Vector3(
            RandomFloat(minRange, maxRange) - 0.5f,
            RandomFloat(minRange, maxRange) - 0.5f,
            0.f
        );
    }
}
