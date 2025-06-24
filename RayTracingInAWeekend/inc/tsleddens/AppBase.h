#pragma once
#include "BvhNode.h"
#include "Camera.h"
#include "SDLRasterizer.h"
#include "World.h"

namespace tsleddens
{
class AppBase : public SDLRasterizer
{
    Camera    m_camera;
    World     m_world {};
    BvhNode   m_bvh {};
    ULONGLONG m_sampleCount = 0;

    std::chrono::time_point<std::chrono::steady_clock> m_appStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_currentSampleStartTime;

protected:
    AppBase( const int width, const int height, const wchar_t* title )
    : SDLRasterizer( width, height, title )
    , m_camera( width, height )
    , m_appStartTime( std::chrono::high_resolution_clock::now() )
    , m_currentSampleStartTime( m_appStartTime )
    {}

    ~AppBase() override = default;

    World* GetWorld()
    {
        return &m_world;
    }

    Camera* GetCamera()
    {
        return &m_camera;
    }

    [[nodiscard]] virtual IRayTraceable* GetLights()
    {
        return nullptr;
    }

    virtual void InitWorld( World& world ) = 0;

    void OnInit() final;
    void OnResize( UINT newWidth, UINT newHeight ) final;

    void OnBeforeRender() final
    {
        m_camera.Render( m_bvh, *this, GetLights() );
    }

    void OnAfterRender() final
    {
        UpdateFps();
    }

    void OnUpdate() override {}
    void OnDestroy() override {}

private:
    void UpdateFps();
};
}  // namespace tsleddens