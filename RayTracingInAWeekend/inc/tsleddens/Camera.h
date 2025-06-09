#pragma once
#include "Defines.h"
#include "DeFocusDisk.h"

namespace tsleddens
{
    class Win32Rasterizer;
    class Ray;
    class DeFocusDisk;
    struct IRayTraceable;

    class Camera
    {
        bool m_isRenderNormalsEnabled = false;

        UINT m_imageWidth = 0;
        UINT m_imageHeight = 0;
        UINT m_samplesPerPixel = 1;
        UINT m_frameCount = 1;
        UINT m_maxBounces = 32;

        float m_aspectRatio = 0.f;
        float m_verticalFov = 20.f;

        Vector3 m_uDelta = Vector3(0.f);
        Vector3 m_vDelta = Vector3(0.f);

        Vector3 m_upDirection = Vector3(0.f, 1.f, 0.f);

        Point3 m_lookAt = Vector3(0.f);
        Point3 m_position = Point3(13.f, 2.f, 3.f);
        Point3 m_p0 = Point3(0.f);

        DeFocusDisk m_deFocusDisk;

    public:
        Camera(UINT imageWidth, UINT imageHeight);

        void Resize(UINT imageWidth, UINT imageHeight);

        void SetVerticalFieldOfView(const float value)
        {
            m_verticalFov = value;
            UpdateViewport();
        }

        void SetDeFocusValues(const float distance, const float angle) {
            m_deFocusDisk.SetDistance(distance);
            m_deFocusDisk.SetAngle(angle);
            UpdateViewport();
        }

        void SetLookAt(const Point3& lookAt)
        {
            m_lookAt = lookAt;
            UpdateViewport();
        }

        void SetPosition(const Point3& position)
        {
            m_position = position;
            UpdateViewport();
        }

        void SetMaxBounces(const UINT max) { m_maxBounces = max; }
        void SetSamplesPerPixel(UINT value) { m_samplesPerPixel = value < 1 ? 1 : value; }

        void TraceAndPlot(const IRayTraceable& world, Win32Rasterizer& rasterizer);
        void EnableRenderNormals() { m_isRenderNormalsEnabled = true; }
        void DisableRenderNormals() { m_isRenderNormalsEnabled = false; }

        UINT GetSamplesPerPixel() const { return m_samplesPerPixel; }
        UINT GetMaxBounces() const { return m_maxBounces; }

        bool IsRenderNormalsEnabled() const { return m_isRenderNormalsEnabled; }
    private:
        void UpdateViewport() { Resize(m_imageWidth, m_imageHeight); }
        Ray GetRay(const UINT x, const UINT y) const;
        Color SampleColor(const Ray& ray, const IRayTraceable& world, UINT currentBounces) const;
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
