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
        UINT m_maxBounces = 10;

        float m_aspectRatio = 0.f;
        float m_verticalFov = 20.f;

        Vector3 m_uDelta = Vector3(0.f);
        Vector3 m_vDelta = Vector3(0.f);

        Vector3 m_upDirection = Vector3(0.f, 1.f, 0.f);

        Point3 m_lookAt = Vector3(0.f);
        Point3 m_position = Point3(13.f, 2.f, 3.f);
        Point3 m_p0 = Point3(0.f);

        DeFocusDisk m_deFocusDisk;

        Color m_background = Color(0.f);

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

        void SetLookAt(const float x, const float y, const float z)
        {
            SetLookAt(Point3(x, y, z));
        }

        void SetPosition(const Point3& position)
        {
            m_position = position;
            UpdateViewport();
        }

        void SetPosition(const float x, const float y, const float z)
        {
            SetPosition(Point3(x, y, z));
        }

        void SetBackgroundColor(const Color& backgroundColor) { m_background = backgroundColor; }
        void SetBackgroundColor(const float rgb) { m_background = Color(rgb); }
        void SetBackgroundColor(const float r, const float g, const float b) { m_background = Color(r, g, b); }

        void SetMaxBounces(const UINT max) { m_maxBounces = max; }
        void SetSamplesPerPixel(const UINT value) { m_samplesPerPixel = value < 1 ? 1 : value; }

        void Render(const IRayTraceable& world, Win32Rasterizer& rasterizer, IRayTraceable* lights);
        void EnableRenderNormals() { m_isRenderNormalsEnabled = true; }
        void DisableRenderNormals() { m_isRenderNormalsEnabled = false; }

        [[nodiscard]] UINT GetSamplesPerPixel() const { return m_samplesPerPixel; }
        [[nodiscard]] UINT GetMaxBounces() const { return m_maxBounces; }

        [[nodiscard]] bool IsRenderNormalsEnabled() const { return m_isRenderNormalsEnabled; }
    private:
        void UpdateViewport() { Resize(m_imageWidth, m_imageHeight); }
        [[nodiscard]] Ray GetRay(const UINT x, const UINT y) const;
        [[nodiscard]] Color SampleColor(const Ray& ray, const IRayTraceable& world, UINT currentBounces, IRayTraceable* lights) const;
        static Color GetNormalColor(const Vector3& normal);
        static Color GetNoHitColor(const Ray& ray);
        static Vector3 GenerateRandomOffset(float minRange = 0.f, float maxRange = 1.f);
    };

    inline Vector3 Camera::GenerateRandomOffset(const float minRange, const float maxRange)
    {
        return Vector3(
            RandomFloat(minRange, maxRange) - 0.5f,
            RandomFloat(minRange, maxRange) - 0.5f,
            0.f
        );
    }
}
