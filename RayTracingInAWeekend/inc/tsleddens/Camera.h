#pragma once

namespace tsleddens
{
    class Win32Rasterizer;
    struct IRayTraceable;

    class Camera
    {
        bool m_bRenderNormals = false;

        UINT m_imageWidth = 0;
        UINT m_imageHeight = 0;

        float m_aspectRatio = 0.f;
        float m_fieldOfView = 1.f;
        float m_uDelta = 0.f;
        float m_vDelta = 0.f;

        Point3 m_position = Point3(0.f);
        Point3 m_p0 = Point3(0.f);
        Point3 m_p1 = Point3(0.f);
        Point3 m_p2 = Point3(0.f);

        Vector3 m_viewDirection = Vector3(0.f, 0.f, -1.f);

    public:
        Camera(UINT imageWidth, UINT imageHeight);
        void Resize(UINT imageWidth, UINT imageHeight);
        void SetFieldOfView(const float fov) { m_fieldOfView = fov; }
        void TraceAndPlot(const IRayTraceable& world, const Win32Rasterizer& rasterizer) const;
        void EnableRenderNormals() { m_bRenderNormals = true; }
        void DisableRenderNormals() { m_bRenderNormals = false; }
        bool IsRenderNormalsEnabled() const { return m_bRenderNormals; }
    private:
    };
}