#pragma once
#include "BvhNode.h"
#include "Camera.h"
#include "Win32Rasterizer.h"
#include "World.h"

namespace tsleddens
{
    class AppBase : public Win32Rasterizer
    {
        Camera m_camera;
        World m_world{};
        BvhNode m_bvh{};
        ULONGLONG m_frameCount = 0;

    protected:
        AppBase(int width, int height, const wchar_t* title) :
            Win32Rasterizer(width, height, title),
            m_camera(width, height)
        {
        }
        ~AppBase() override = default;

        World* GetWorld() { return &m_world; }
        Camera* GetCamera() { return &m_camera; }

        virtual void InitWorld(World& world) = 0;

        void OnInit() final;
        void OnResize(UINT newWidth, UINT newHeight) final;
        void OnBeforeRender() final { m_camera.TraceAndPlot(m_bvh, *this); }
        void OnAfterRender() final { UpdateFps(); }

        void OnUpdate() override {}
        void OnDestroy() override {}

    private:
        void UpdateFps();
    };
}
